#include <windows.h>
#include <cpctelera.h>

#define WINDOW_STYLE	WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE
#define LARGEUR_ECRAN	320
#define HAUTEUR_ECRAN	200
#define TITLE			"Bitume"			

static HWND _hWnd;
static u16 _curKey;

typedef void(*TInterrupt)(void);

void MsgLoop();

struct SAmstrad
{
	u8 _mode;
	u8 _currentPage;
	TInterrupt _interruptFunction;

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

}

void cpct_setPALColour(u8 pen, u8 hw_ink)
{

}

u8* cpct_getScreenPtr(void* screen_start, u8 x, u8 y)
{
	static u8 pos[2];
	pos[0] = x;
	pos[1] = y;

	return (u8*)pos;
}

void cpct_waitVSYNC()
{

}

void drawSprite(void *sprite, int x, int y, int cx, int cy)
{
	int coef = 4;
	if (_amstrad._mode == 0)
		coef = 4;
	else
		coef = 8;

	RECT rect = { x * coef, y, (x + cx) * coef, y + cy };

	UINT color = (UINT)sprite;
	COLORREF rgb = RGB(GetRValue(color), GetGValue(color), GetBValue(color));
	
	HDC hdc = GetDC(_hWnd);
	HBRUSH brush = CreateSolidBrush(rgb);
	FillRect(hdc, &rect, brush);
	DeleteObject(brush);

	ReleaseDC(_hWnd, hdc);
}

void cpct_drawSprite(void *sprite, void* memory, u8 width, u8 height)
{
	u8* mem = (u8*)memory;
	drawSprite(sprite, mem[0], mem[1], width, height);
}

void cpct_drawSpriteMasked(void *sprite, void* memory, u8 width, u8 height)
{
	u8* mem = (u8*)memory;
	drawSprite(sprite, mem[0], mem[1], width, height);
}

void cpct_drawSolidBox(void *memory, u8 colour_pattern, u8 width, u8 height)
{
	u8* mem = (u8*)memory;
	drawSprite(0xFFFFFFFF, mem[0], mem[1], width, height);
}


u8 cpct_px2byteM0(u8 px0, u8 px1)
{
	return 0;
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
	ZeroMemory(&rcDesiredWindowRect, sizeof(rcDesiredWindowRect));

	// Get the current window rect and its client rect
	RECT rcCurrentWindowRect;
	RECT rcCurrentClientRect;

	GetWindowRect(hWindow, &rcCurrentWindowRect);
	GetClientRect(hWindow, &rcCurrentClientRect);

	// Get the difference between the current and desired client areas
	SIZE szClientDifference;
	szClientDifference.cx = (rcCurrentClientRect.right - szDesiredClient.cx);
	szClientDifference.cy = (rcCurrentClientRect.bottom - szDesiredClient.cy);

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

	int posX = (GetSystemMetrics(SM_CXFULLSCREEN) - LARGEUR_ECRAN) / 2;
	int posY = (GetSystemMetrics(SM_CYFULLSCREEN) - HAUTEUR_ECRAN) / 2;

	SIZE szDesiredClient = { LARGEUR_ECRAN, HAUTEUR_ECRAN };

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

	SetWindowText(_hWnd, "test");
	PosWindow();
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

void CPCTeleraWin()
{
	ZeroMemory(&_amstrad, sizeof(_amstrad));

	CreateWindowApp();
	MsgLoop();
}
