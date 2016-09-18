#include <windows.h>
#include <cpctelera.h>

#define WINDOW_STYLE	WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE
#define BORDER_CX		32
#define BORDER_UP_CY	48
#define BORDER_DW_CY	40
#define WIDTH_SCREEN	320
#define HEIGHT_SCREEN	200
#define FULL_SCREEN_CX	(BORDER_CX + WIDTH_SCREEN + BORDER_CX)
#define FULL_SCREEN_CY	(BORDER_UP_CY + HEIGHT_SCREEN + BORDER_DW_CY)

#define NB_COLORS		16
#define TITLE			"WinCPCTelera"			

static HBITMAP _hBitmap;
static HPALETTE _hPal;
static HWND _hWnd;
static u16 _curKey;

typedef void(*TInterrupt)(void);

void MsgLoop();
void CreatePaletteCpc();
void DisplayBitmap(HDC hdc, int x, int y, int cx, int cy, char* data, BOOL pMasked);

typedef struct 
{
	UCHAR hw;
	COLORREF rgb;
} SCPCPalette;

const SCPCPalette _palette[27] =
{
	HW_BLACK, RGB(0,0,0),
	HW_BLUE, RGB(0,0,128),
	HW_BRIGHT_BLUE, RGB(0,0,255),
	HW_RED, RGB(128,0,0),
	HW_MAGENTA, RGB(128,0,128),
	HW_MAUVE, RGB(128,0,255),
	HW_BRIGHT_RED, RGB(255,0,0),
	HW_PURPLE, RGB(255,0,128),
	HW_BRIGHT_MAGENTA, RGB(255,0,255),
	HW_GREEN, RGB(0,128,0),
	HW_CYAN, RGB(128,0,128),
	HW_SKY_BLUE, RGB(0,128,255),
	HW_YELLOW, RGB(128,128,0),
	HW_WHITE, RGB(128,128,128),
	HW_PASTEL_BLUE, RGB(128,128,255),
	HW_ORANGE, RGB(255,128,0),
	HW_PINK, RGB(255,128,128),
	HW_PASTEL_MAGENTA, RGB(255,128,255),
	HW_BRIGHT_GREEN, RGB(0,255,0),
	HW_SEA_GREEN, RGB(0,255,128),
	HW_BRIGHT_CYAN, RGB(0,255,255),
	HW_LIME, RGB(128,255,0),
	HW_PASTEL_GREEN, RGB(128,255,128),
	HW_PASTEL_CYAN, RGB(128,255,255),
	HW_BRIGHT_YELLOW, RGB(255,255,0),
	HW_PASTEL_YELLOW, RGB(255,255,128),
	HW_BRIGHT_WHITE, RGB(255,255,255)
};

COLORREF GetColorHW(int pHW)
{
	for (int i = 0; i < 27; i++)
	{
		if (_palette[i].hw == pHW)
			return _palette[i].rgb;
	}
	return _palette[0].rgb;
}

COLORREF GetColorFW(int pFW)
{
	return _palette[pFW].rgb;
}

struct SAmstrad
{
	u8 _mode;
	u8 _currentPage;
	TInterrupt _interruptFunction;
	u8 _rowVideo[80];
	u8 _y;

	HBITMAP _video;

	UCHAR _curPal[NB_COLORS];
	UCHAR _mem[0xFFFF];
}
_amstrad;

u8* getMemory(int pAddress)
{
	if (pAddress > 0xFFFF)
		return (u8*)pAddress;
	else
		return &_amstrad._mem[pAddress];
}

void cpct_memcpy(void* to, const void* from, u16 size)
{
	u8* toMem = getMemory((int)to);
	u8* fromMem = getMemory((int)from);

	memcpy_s(toMem, size, fromMem, size);
}

void cpct_memset(void *array, u8  value, u16 size)
{
	u8* arrayMem = getMemory((int)array);
	memset(arrayMem, value, size);
}

void cpct_setStackLocation(void* memory)
{

}

u8 cpct_rand8()
{
	return (u8)rand();
}

VOID CALLBACK MyTimerProc(
	HWND hwnd,
	UINT message,
	UINT idTimer,
	DWORD dwTime)
{
	_amstrad._interruptFunction();
}

void cpct_setInterruptHandler(void(*intHandler)(void))
{
	_amstrad._interruptFunction = intHandler;
	SetTimer(_hWnd, 10000, 33, MyTimerProc);
}

void cpct_reenableFirmware(u16 firmware_ROM_code)
{
}

u16 cpct_disableFirmware()
{
	srand(GetTickCount());
	CPCTeleraWin();
	return 0;
}

void cpct_scanKeyboard_f()
{
	MsgLoop();
}

void cpct_scanKeyboard_if()
{
	MsgLoop();
}

void cpct_scanKeyboard()
{
	MsgLoop();
}

u8 cpct_isKeyPressed(cpct_keyID key)
{
	MsgLoop();
	return (_curKey == key);
}

u8 cpct_isAnyKeyPressed_f()
{
	MsgLoop();
	return (_curKey != 0);
}

void cpct_setPalette(u8* ink_array, u16 ink_array_size)
{
	memcpy(_amstrad._curPal, ink_array, ink_array_size);
	CreatePaletteCpc();
}

void cpct_setPALColour(u8 pen, u8 hw_ink)
{
	_amstrad._curPal[pen] = hw_ink;
	CreatePaletteCpc();
}

u8* cpct_getScreenPtr(void* screen_start, u8 x, u8 y)
{
	for (int i = 0; i < 80; i++)
		_amstrad._rowVideo[i] = x + i;

	_amstrad._y = y;

	return _amstrad._rowVideo;
}

void cpct_waitVSYNC()
{

}

void SetPalette(int i, UCHAR pHW)
{
	_amstrad._curPal[i] = pHW;
}

void fillBox(HDC hdc, int index, int x, int y, int cx, int cy)
{
	int hw = _amstrad._curPal[index];
	COLORREF rgb = GetColorHW(hw);
	HBRUSH brush = CreateSolidBrush(rgb);
	
	if (_amstrad._mode == 0)
	{
		x *= 4;
		cx *= 4;
	}

	RECT rect = { x, y, x + cx, y + cy };

	FillRect(hdc, &rect, brush);
	DeleteObject(brush);
}

void drawSprite(void *sprite, int x, int y, int cx, int cy, BOOL pMasked)
{
	HDC hdc = GetDC(_hWnd);
	HDC memDC = CreateCompatibleDC(hdc);

	HBITMAP oldBitmap = SelectObject(memDC, _amstrad._video);
	
	DisplayBitmap(memDC, x, y, cx, cy, sprite, pMasked);
	BitBlt(hdc, BORDER_CX, BORDER_UP_CY, WIDTH_SCREEN, HEIGHT_SCREEN, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, oldBitmap);
	DeleteDC(memDC);
	ReleaseDC(_hWnd, hdc);
}

void cpct_drawSprite(void *sprite, void* memory, u8 width, u8 height)
{
	u8* x = (u8*)memory;
	drawSprite(sprite, *x, _amstrad._y, width, height, FALSE);
}

void cpct_drawSpriteMasked(void *sprite, void* memory, u8 width, u8 height)
{
	u8* x = (u8*)memory;
	drawSprite(sprite, *x, _amstrad._y, width, height, TRUE);
}

void cpct_drawSolidBox(void *memory, u8 colour_pattern, u8 width, u8 height)
{
	HDC hdc = GetDC(_hWnd);
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP oldBitmap = SelectObject(memDC, _amstrad._video);

	u8* x = (u8*)memory;
	fillBox(memDC, colour_pattern, *x, _amstrad._y, width, height);

	BitBlt(hdc, BORDER_CX, BORDER_UP_CY, WIDTH_SCREEN, HEIGHT_SCREEN, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, oldBitmap);
	DeleteDC(memDC);
	ReleaseDC(_hWnd, hdc);
}

u8 cpct_px2byteM0(u8 px0, u8 px1)
{
	UCHAR bit0 = (px0 & 0x01);
	UCHAR bit1 = (px0 & 0x02) >> 1;
	UCHAR bit2 = (px0 & 0x04) >> 2;
	UCHAR bit3 = (px0 & 0x08) >> 3;
	UCHAR bita = (px1 & 0x01);
	UCHAR bitb = (px1 & 0x02) >> 1;
	UCHAR bitc = (px1 & 0x04) >> 2;
	UCHAR bitd = (px1 & 0x08) >> 3;

	return bit0 << 7 | bita << 6 | bit2 << 5 | bitc << 4 | bit1 << 3 | bitb << 2 | bit3 << 2 | bitd;
}

void cpct_setVideoMode(u8 videoMode)
{
	_amstrad._mode = videoMode;
}

void cpct_setVideoMemoryPage(u8 page_6LSb)
{
	_amstrad._currentPage = page_6LSb;
}

void cpc_UnExo(const u8 *source, u8* dest)
{

}

void HandleKeyboard(UINT key)
{
	switch (key)
	{
	case VK_UP:
		_curKey = Key_CursorUp;
		break;

	case VK_DOWN:
		_curKey = Key_CursorDown;
		break;

	case VK_LEFT:
		_curKey = Key_CursorLeft;
		break;

	case VK_RIGHT:
		_curKey = Key_CursorRight;
		break;

	case VK_SPACE:
		_curKey = Key_Space;
		break;

	case VK_CONTROL:
		_curKey = Key_Copy;
		break;

	default:
		_curKey = 0;
		break;
	}
}

LRESULT FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		HandleKeyboard((UINT)wParam);
		break;

	case WM_DESTROY:
		exit(0);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
RECT CalculateWindowRect(HWND hWindow, SIZE szDesiredClient)
{
	// Declare a RECT to hold the results of our calculations
	RECT rcDesiredWindowRect;

	// Get the current window rect and its client rect
	RECT rcCurrentWindowRect;
	RECT rcCurrentClientRect;

	GetWindowRect(hWindow, &rcCurrentWindowRect);
	GetClientRect(hWindow, &rcCurrentClientRect);

	// Get the difference between the current and desired client areas
	SIZE szClientDifference = { rcCurrentClientRect.right - szDesiredClient.cx, rcCurrentClientRect.bottom - szDesiredClient.cy };

	// Get the difference between the current window rect and the desired window rect
	rcDesiredWindowRect.left = rcCurrentWindowRect.left;
	rcDesiredWindowRect.top = rcCurrentWindowRect.top;
	rcDesiredWindowRect.right = (rcCurrentWindowRect.right - szClientDifference.cx);
	rcDesiredWindowRect.bottom = (rcCurrentWindowRect.bottom - szClientDifference.cy);

	return rcDesiredWindowRect;
}

void PosWindow()
{
	SetWindowLong(_hWnd, GWL_STYLE, WINDOW_STYLE);
	MoveWindow(_hWnd, 0, 0, 0, 0, FALSE);

	int posX = (GetSystemMetrics(SM_CXFULLSCREEN) - FULL_SCREEN_CX) / 2 + 100;
	int posY = (GetSystemMetrics(SM_CYFULLSCREEN) - FULL_SCREEN_CY) / 2;

	SIZE szDesiredClient = { FULL_SCREEN_CX, FULL_SCREEN_CY };

	RECT rcNewWindowRect = CalculateWindowRect(_hWnd, szDesiredClient);

	SIZE size = { rcNewWindowRect.right - rcNewWindowRect.left,
		rcNewWindowRect.bottom - rcNewWindowRect.top };

	MoveWindow(_hWnd, posX, posY, size.cx, size.cy, TRUE);
}

void CreateWindowApp()
{
	HINSTANCE instance = GetModuleHandle(NULL);

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = TITLE;
	wc.lpszClassName = TITLE;

	RegisterClass(&wc);

	_hWnd = CreateWindow(TITLE,
		NULL,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
		0, 0,
		0, 0,
		NULL,
		NULL,
		instance,
		NULL);

	SetWindowText(_hWnd, TITLE);
	PosWindow();

	_hPal = NULL;
	CreatePaletteCpc();
}

void CreatePaletteCpc()
{
	NPLOGPALETTE logPalette = (NPLOGPALETTE)malloc(sizeof(LOGPALETTE) + NB_COLORS * sizeof(PALETTEENTRY));

	logPalette->palNumEntries = NB_COLORS;
	logPalette->palVersion = 0x300;

	for (int i = 0; i < NB_COLORS; i++)
	{
		int hw = _amstrad._curPal[i];
		COLORREF rgb = GetColorHW(hw);

		logPalette->palPalEntry[i].peBlue = GetBValue(rgb);
		logPalette->palPalEntry[i].peRed = GetRValue(rgb);
		logPalette->palPalEntry[i].peGreen = GetGValue(rgb);
		logPalette->palPalEntry[i].peFlags = 0;
	}

	if (_hPal != NULL)
		DeleteObject(_hPal);

	_hPal = CreatePalette(logPalette);

	free(logPalette);
}

void FillBorder(HDC hdc)
{
	int hw = _amstrad._curPal[0];
	COLORREF rgb = GetColorHW(hw);
	HBRUSH brush = CreateSolidBrush(rgb);

	RECT top = { 0, 0, BORDER_CX + WIDTH_SCREEN + BORDER_CX, BORDER_UP_CY };
	FillRect(hdc, &top, brush);

	RECT left = { 0, BORDER_UP_CY, BORDER_CX, BORDER_UP_CY + HEIGHT_SCREEN + BORDER_DW_CY };
	FillRect(hdc, &left, brush);

	RECT bottom = { 0, BORDER_UP_CY + HEIGHT_SCREEN, BORDER_CX + WIDTH_SCREEN + BORDER_CX, BORDER_UP_CY + HEIGHT_SCREEN + BORDER_DW_CY };
	FillRect(hdc, &bottom, brush);

	RECT right = { BORDER_CX + WIDTH_SCREEN, BORDER_UP_CY, BORDER_CX + WIDTH_SCREEN + BORDER_CX, BORDER_UP_CY + HEIGHT_SCREEN };
	FillRect(hdc, &right, brush);

	DeleteObject(brush);
}

void FillScreen(HDC hdc)
{
	HBRUSH brush = CreateSolidBrush(_palette[2].rgb);

	RECT rect = { BORDER_CX, BORDER_UP_CY, BORDER_CX + WIDTH_SCREEN, BORDER_UP_CY + HEIGHT_SCREEN };
	FillRect(hdc, &rect, brush);

	DeleteObject(brush);
}

UCHAR DecodePixel(UCHAR pPix)
{
	UCHAR pix0 = (pPix & 0x80) >> 7;
	UCHAR pixa = (pPix & 0x40) >> 6;
	UCHAR pix2 = (pPix & 0x20) >> 5;
	UCHAR pixc = (pPix & 0x10) >> 4;
	UCHAR pix1 = (pPix & 0x08) >> 3;
	UCHAR pixb = (pPix & 0x04) >> 2;
	UCHAR pix3 = (pPix & 0x02) >> 1;
	UCHAR pixd = (pPix & 0x01);

	return (pix3 << 7 | pix2 << 6 | pix1 << 5 | pix0 << 4 | pixd << 3 | pixc << 2 | pixb << 1 | pixa);
}

void DisplayBitmap(HDC hdc, int x, int y, int cx, int cy, char* data, BOOL pMasked)
{
	int xi, yi;
	int sizeBitmapInfo = sizeof(BITMAPINFOHEADER) + 16 * sizeof(WORD);
	LPBITMAPINFO bitmapInfos = (LPBITMAPINFO)malloc(sizeBitmapInfo);
	WORD* pal = NULL;

	int widthAlignedDWORD = (((cx) * 8 + 31)  & (~31)) / 8;
	UCHAR* alignedData = NULL;
	UCHAR* sprite = NULL;
	UCHAR* mask = NULL;

	if (pMasked)
	{
		sprite = malloc(widthAlignedDWORD * cy);
		ZeroMemory(sprite, widthAlignedDWORD * cy);

		mask = malloc(widthAlignedDWORD * cy);
		ZeroMemory(mask, widthAlignedDWORD * cy);
		
		WORD* pix = (WORD*)data;
		int i = 0;
		for (yi = 0; yi < cy; yi++)
		{
			for (xi = 0; xi < cx; xi++)
			{
				mask[i] = DecodePixel((UCHAR)(*pix));
				sprite[i] = DecodePixel((UCHAR)(*pix >> 8));
				
				pix++;
				i += (widthAlignedDWORD - cx + 1);
			}
		}
	}
	else
	{
		alignedData = malloc(widthAlignedDWORD * cy);
		ZeroMemory(alignedData, widthAlignedDWORD * cy);

		for (yi = 0; yi < cy; yi++)
			memcpy(alignedData + yi * widthAlignedDWORD, data + yi * cx, cx);

		for (yi = 0; yi < cy; yi++)
		{
			UCHAR* pix = alignedData + yi * widthAlignedDWORD;
			for (xi = 0; xi < cx; xi++)
			{
				*pix = DecodePixel(*pix);
				pix++;
			}
		}
	}

	int cxDest = cx;
	int coef = 1;
	if (_amstrad._mode == 0)
	{
	//	coef = 2;
		x *= 2;
		cx *= 2;
	}

	memset(bitmapInfos, 0, sizeof(BITMAPINFOHEADER));
	bitmapInfos->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfos->bmiHeader.biWidth = cx;
	bitmapInfos->bmiHeader.biHeight = -cy;
	bitmapInfos->bmiHeader.biPlanes = 1;
	bitmapInfos->bmiHeader.biBitCount = 4;
	bitmapInfos->bmiHeader.biCompression = BI_RGB;
	bitmapInfos->bmiHeader.biClrUsed = NB_COLORS;

	pal = (WORD*)bitmapInfos->bmiColors;
	for (int i = 0; i < NB_COLORS; i++)
		pal[i] = i;

	SelectPalette(hdc, _hPal, FALSE);
	RealizePalette(hdc);

	if (!pMasked)
	{
		StretchDIBits(hdc, x * 2, y, cx * 2, cy, 0, 0, cx, cy, alignedData, bitmapInfos, DIB_PAL_COLORS, SRCCOPY);
		free(alignedData);
	}
	else
	{
		StretchDIBits(hdc, x * 2, y, cx * 2, cy, 0, 0, cx, cy, sprite, bitmapInfos, DIB_PAL_COLORS, SRCINVERT);
		StretchDIBits(hdc, x * 2, y, cx * 2, cy, 0, 0, cx, cy, mask, bitmapInfos, DIB_PAL_COLORS, SRCAND);
		StretchDIBits(hdc, x * 2, y, cx * 2, cy, 0, 0, cx, cy, sprite, bitmapInfos, DIB_PAL_COLORS, SRCINVERT);

		free(mask);
		free(sprite);
	}
}

void MsgLoop()
{
	MSG msg;
	memset(&msg, 0, sizeof(msg));

	for (int i = 0; i < 10; i++)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			return;
	}
}

void StartCPC()
{
	ZeroMemory(&_amstrad, sizeof(_amstrad));
	for (int i = 0; i < NB_COLORS; i++)
		_amstrad._curPal[i] = _palette[i].hw;

	_amstrad._curPal[0] = HW_BRIGHT_BLUE;
	_amstrad._curPal[1] = HW_BRIGHT_YELLOW;

	HDC hdc = GetDC(_hWnd);

	_amstrad._video = CreateCompatibleBitmap(hdc, WIDTH_SCREEN, HEIGHT_SCREEN);

	FillBorder(hdc);

	ReleaseDC(_hWnd, hdc);
}

void CPCTeleraWin()
{
	StartCPC();

	CreateWindowApp();
	MsgLoop();
}
