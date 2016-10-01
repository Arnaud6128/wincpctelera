#include "winCpctelera.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

HWND _hWnd;
BOOL _curKey;
SAmstrad _amstrad;

static int _widthVideo;
static HBITMAP _hBitmap;
static BOOL _isStarted = FALSE;

#define ALIGNED_DWORD(CX) (((CX) * 8 + 31)  & (~31)) / 8

//LRESULT FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern void CreateWindowApp();


const SCPCPalette _palette[NB_PAL_COLOR] =
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

#define FONT_SIZE		8
#define FONT_NB_LINE	32
#define FONT_NB_COL		3
#define FONT_COL		(FONT_NB_COL*FONT_SIZE)

/** Bottom - Top */
const UCHAR fonts[] = 
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x80, 
	0x3B, 0x6E, 0x1E, 0x3B, 0x1E, 0x3C, 0x03, 0x73, 0x1E, 0x33, 0x73, 0x1E, 0x63, 0x33, 0x1E, 0x30, 0x06, 0x78, 0x3E, 0x0E, 0x1F, 0x0C, 0x36, 0x63, 0x03, 0x3F, 0x07, 0x0C, 0x38, 0x00, 0x66, 0x00,
	0x66, 0x33, 0x33, 0x66, 0x30, 0x18, 0x1F, 0x33, 0x0C, 0x33, 0x36, 0x0C, 0x6B, 0x33, 0x33, 0x3E, 0x3E, 0x30, 0x03, 0x1B, 0x33, 0x1E, 0x7F, 0x36, 0x1F, 0x19, 0x0C, 0x0C, 0x0C, 0x00, 0x19, 0x80, 
	0x3E, 0x33, 0x30, 0x66, 0x3F, 0x18, 0x33, 0x33, 0x0C, 0x03, 0x3C, 0x0C, 0x6B, 0x33, 0x33, 0x33, 0x66, 0x30, 0x1E, 0x18, 0x33, 0x33, 0x6B, 0x1C, 0x33, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x66, 0x00, 
	0x06, 0x33, 0x33, 0x66, 0x33, 0x3C, 0x33, 0x3B, 0x0C, 0x03, 0x36, 0x0C, 0x7F, 0x33, 0x33, 0x33,	0x66, 0x3B, 0x30, 0x18, 0x33, 0x33, 0x6B, 0x36, 0x33, 0x26, 0x38, 0x0C, 0x07, 0x00, 0x19, 0x80, 
	0x3C, 0x3E, 0x1E, 0x3E, 0x1E, 0x18, 0x1F, 0x36,	0x1C, 0x07, 0x33, 0x0C, 0x36, 0x6E, 0x1E, 0x6E, 0x3B, 0x6E, 0x1E, 0x3E, 0x33, 0x33, 0x63, 0x63, 0x33, 0x3F, 0x0C, 0x0C, 0x0C, 0x00, 0x66, 0x00, 
	0x00, 0x30, 0x00, 0x06, 0x00, 0x1B, 0x00, 0x30, 0x00, 0x00, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x6E, 0x19, 0x80, 
	0x00, 0x70, 0x00, 0x0E, 0x00, 0x0E, 0x00, 0x70, 0x0C, 0x03, 0x70, 0x1C, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x38, 0x3B, 0x66, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 
	0x33, 0x7E, 0x1E, 0x7C, 0x7F, 0x78, 0x1F, 0x33, 0x3F, 0x3C, 0x73, 0x7F, 0x63, 0x63, 0x1C, 0x78, 0x3B, 0x73, 0x1E, 0x1E, 0x1E, 0x0C, 0x63, 0x63, 0x1E, 0x7F, 0x1E, 0x01, 0x1E, 0x0C, 0x00, 0x00, 
	0x33, 0x33, 0x33, 0x36, 0x31, 0x30, 0x33, 0x33, 0x0C, 0x66, 0x33, 0x33, 0x63, 0x63, 0x36, 0x30, 0x66, 0x33, 0x33, 0x0C, 0x33, 0x1E, 0x77, 0x63, 0x0C, 0x33, 0x18, 0x03, 0x06, 0x0C, 0x00, 0x00, 
	0x3F, 0x33, 0x60, 0x33, 0x34, 0x34, 0x67, 0x33, 0x0C, 0x66, 0x36, 0x31, 0x6B, 0x67, 0x63, 0x30, 0x6D, 0x36, 0x03, 0x0C, 0x33, 0x33, 0x7F, 0x36, 0x0C, 0x19, 0x18, 0x06, 0x06, 0x0C, 0x00, 0x00, 
	0x33, 0x3E, 0x60, 0x33, 0x3C, 0x3C, 0x60, 0x3F, 0x0C, 0x06, 0x3C, 0x30, 0x7F, 0x6F, 0x63, 0x3E, 0x63, 0x3E, 0x1E, 0x0C, 0x33, 0x33, 0x6B, 0x1C, 0x1E, 0x0C, 0x18, 0x0C, 0x06, 0x0C, 0x00, 0x00, 
	0x33, 0x33, 0x60, 0x33, 0x34, 0x34, 0x60, 0x33, 0x0C, 0x06, 0x36, 0x30, 0x7F, 0x7B, 0x63, 0x33, 0x63, 0x33, 0x30, 0x0C, 0x33, 0x33, 0x63, 0x1C, 0x33, 0x46, 0x18, 0x18, 0x06, 0x3F, 0x00, 0x06,
	0x1E, 0x33, 0x33, 0x36, 0x31, 0x31, 0x33, 0x33, 0x0C, 0x06, 0x33, 0x30, 0x77, 0x73, 0x36, 0x33, 0x36, 0x33, 0x33, 0x2D, 0x33, 0x33, 0x63, 0x36, 0x33, 0x63, 0x18, 0x30, 0x06, 0x1E, 0x00, 0x0C, 
	0x0C, 0x7E, 0x1E, 0x7C, 0x7F, 0x7F, 0x1E, 0x33, 0x3F, 0x0F, 0x73, 0x78, 0x63, 0x63, 0x1C, 0x7E, 0x1C, 0x7E, 0x1E, 0x3F, 0x33, 0x33, 0x63, 0x63, 0x33, 0x7F, 0x1E, 0x60, 0x1E, 0x0C, 0x00, 0x18, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x0C, 0x00, 0x36, 0x0C, 0x63, 0x3B, 0x00, 0x06, 0x18, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x40, 0x3E, 0x3F, 0x3F, 0x1E, 0x0F, 0x1E, 0x1E, 0x0C, 0x1E, 0x1E, 0x0C, 0x0C, 0x06, 0x00, 0x30, 0x0C, 0x3E, 
	0x00, 0x00, 0x36, 0x3E, 0x33, 0x66, 0x00, 0x0C, 0x0C, 0x33, 0x0C, 0x0C, 0x00, 0x0C, 0x60, 0x63, 0x0C, 0x33, 0x33, 0x06, 0x33, 0x33, 0x0C, 0x33, 0x33, 0x0C, 0x0C, 0x0C, 0x3F, 0x18, 0x00, 0x60, 
	0x0C, 0x00, 0x7F, 0x0D, 0x18, 0x6E, 0x00, 0x18, 0x06, 0x1E, 0x0C, 0x00, 0x00, 0x00, 0x30, 0x73, 0x0C, 0x30, 0x03, 0x7F, 0x03, 0x33, 0x0C, 0x33, 0x03, 0x00, 0x00, 0x18, 0x00, 0x0C, 0x0C, 0x6F, 
	0x0C, 0x00, 0x36, 0x1E, 0x0C, 0x3B, 0x00, 0x18, 0x06, 0x7F, 0xBF, 0x00, 0x3F, 0x00, 0x18, 0x6B, 0x0C, 0x1E, 0x0E, 0x66, 0x3E, 0x3E, 0x06, 0x1E, 0x1F, 0x0C, 0x0C, 0x30, 0x00, 0x06, 0x06, 0x6F, 
	0x0C, 0x36, 0x7F, 0x2C, 0x66, 0x1C, 0x18, 0x18, 0x06, 0x1E, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x67, 0x0C, 0x03, 0x03, 0x36, 0x30, 0x30, 0x03, 0x33, 0x33, 0x0C, 0x0C, 0x18, 0x3F, 0x0C, 0x33, 0x6F, 
	0x0C, 0x36, 0x36, 0x1F, 0x63, 0x36, 0x0C, 0x0C, 0x0C, 0x33, 0x0C, 0x00, 0x00, 0x00, 0x06, 0x63, 0x1C, 0x33, 0x33, 0x1E, 0x31, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0x0C, 0x00, 0x18, 0x33, 0x63, 
	0x0C, 0x36, 0x36, 0x0C, 0x00, 0x1C, 0x0C, 0x06, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x3E, 0x0C, 0x1E, 0x1E, 0x0E, 0x3F, 0x1E, 0x3F, 0x1E, 0x1E, 0x00, 0x00, 0x06, 0x00, 0x30, 0x1E, 0x3E
};

typedef struct
{
	char cpcChara;
	u8 posBitmap;
} SCharMapping;

const char charaMapBitmap[] = "!'\\#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\'abcdefghijklmnopqrstuvwxyz{|}~";

typedef struct
{
	u16 cpcKeyID;
	u16 winKeyID;
} SKeyMapping;

SKeyMapping cpctMapKey[] =
{
	{ Key_CursorUp, VK_UP },
	{ Key_CursorRight, VK_RIGHT },
	{ Key_CursorDown, VK_DOWN },
	{ Key_F9, VK_F9 },
	{ Key_F6, VK_F6 },
	{ Key_F3, VK_F3 },
	{ Key_Enter, VK_RETURN },
	{ Key_FDot, VK_OEM_PERIOD },

	{ Key_CursorLeft, VK_LEFT },
	{ Key_Copy, VK_MENU },
	{ Key_F7, VK_F7 },
	{ Key_F8, VK_F8 },
	{ Key_F5, VK_F5 },
	{ Key_F1, VK_F1 },
	{ Key_F2, VK_F2 },
	{ Key_F0, VK_F10 },

	{ Key_Clr,  VK_CONTROL },
	{ Key_OpenBracket, 0 },
	{ Key_Return, VK_RETURN },
	{ Key_CloseBracket, 0 },
	{ Key_F4, VK_F4 },
	{ Key_Shift, VK_SHIFT },
	{ Key_BackSlash, VK_BACK },
	{ Key_Control, VK_CONTROL },

	{ Key_Caret, 0 },
	{ Key_Hyphen, 0 },
	{ Key_At, 0 },
	{ Key_P, 0x50 },
	{ Key_SemiColon, 0 },
	{ Key_Colon, 0 },
	{ Key_Slash, 0 },
	{ Key_Dot, VK_OEM_PERIOD },

	{ Key_0, 0x30 },
	{ Key_9, 0x39 },
	{ Key_O, 0x39 },
	{ Key_I, 0x39 },
	{ Key_L, 0x4C },
	{ Key_K, 0x4B },
	{ Key_M, 0x4B },
	{ Key_Comma, VK_OEM_COMMA },

	{ Key_8, 0x4B },
	{ Key_7, 0x37 },
	{ Key_U, 0x55 },
	{ Key_Y, 0x59 },
	{ Key_H, 0x48 },
	{ Key_J, 0x4A },
	{ Key_N, 0x4E },
	{ Key_Space, VK_SPACE },

	{ Key_6, 0x36 },
	{ Joy1_Up, VK_UP },
	{ Key_5, 0x35 },
	{ Joy1_Down, VK_DOWN },
	{ Key_R, 0x52 },
	{ Joy1_Left, VK_LEFT },
	{ Key_T, 0x54 },
	{ Joy1_Right, VK_RIGHT },
	{ Key_G, 0x47 },
	{ Joy1_Fire1, VK_SPACE },
	{ Key_F, 0x46 },
	{ Joy1_Fire2, VK_CONTROL },
	{ Key_B, 0x42 },
	{ Joy1_Fire3, VK_CONTROL },
	{ Key_V, 0x56 },

	{ Key_4, 0x34 },
	{ Key_3, 0x33 },
	{ Key_E, 0x45 },
	{ Key_W, 0x57 },
	{ Key_S, 0x53 },
	{ Key_D, 0x44 },
	{ Key_C, 0x43 },
	{ Key_X, 0x58 },

	{ Key_1, 0x31 },
	{ Key_2, 0x32 },
	{ Key_Esc, VK_ESCAPE },
	{ Key_Q, 0x51 },
	{ Key_Tab, VK_TAB },
	{ Key_A, 0x41 },
	{ Key_CapsLock, VK_CAPITAL },
	{ Key_Z, 0x5A },

	{ Joy0_Up, VK_UP },
	{ Joy0_Down, VK_DOWN },
	{ Joy0_Left, VK_LEFT },
	{ Joy0_Right, VK_RIGHT },
	{ Joy0_Fire1, VK_SPACE },
	{ Joy0_Fire2, VK_CONTROL },
	{ Joy0_Fire3, VK_CONTROL },
	{ Key_Del, VK_DELETE },
};

int GetPixelBit()
{
	switch (_amstrad._mode)
	{
		case MODE_2	: return 1;
		case MODE_1	: return 4; // Hack BMP not handle 2bits bitmap
		case MODE_0	:
		default		: return 4;
	}
}

int GetScreenWidth()
{
	switch (_amstrad._mode)
	{
		case MODE_2	: return 640;
		case MODE_1	: return 320;
		case MODE_0	:
		default		: return 160;		
	}
}

int ConvertPixelPos(int x)
{
	switch (_amstrad._mode)
	{
		case MODE_2	: return x;
		case MODE_1	: return x * 2;
		case MODE_0	:
		default		: return x * 4;
	}
}

BOOL IsCpcMem(const void* pAddress)
{
	return ((int)pAddress < 0xFFFF);
}

void SetPalette(int i, UCHAR pHW)
{
	_amstrad._curPal[i] = pHW;
}

COLORREF GetColorHW(int pHW)
{
	for (int i = 0; i < NB_PAL_COLOR; i++)
	{
		if (_palette[i].hw == pHW)
			return _palette[i].rgb;
	}
	return _palette[0].rgb;
}



u8* GetVideoBufferFromAddress(int pScreenAddr)
{
	switch (pScreenAddr)
	{
	case 0x0000:
		return _amstrad._memVideo[0];
	case 0x4000:
		return _amstrad._memVideo[1];
	case 0x8000:
		return _amstrad._memVideo[2];
	case 0xC000:
	default:
		return _amstrad._memVideo[3];
	}
}

u8* GetVideoBufferFromPage(int pPage)
{
	switch (pPage)
	{
		case cpct_page00:
			return _amstrad._memVideo[0];
		case cpct_page40:
			return _amstrad._memVideo[1];
		case cpct_page80:
			return _amstrad._memVideo[2];
		case cpct_pageC0:
		default:
			return _amstrad._memVideo[3];
	}
}

u8* GetCurVideoBuffer()
{
	return GetVideoBufferFromPage(_amstrad._currentPage);
}

int FindCharaIndex(char pChara)
{
	for (int i = 0; i < sizeof(charaMapBitmap); i++)
	{
		if (charaMapBitmap[i] == pChara)
			return i;
	}
	return 0;
}

u16 GetVKey(u16 pCpcKeyID)
{
	for (int i = 0; i < sizeof(cpctMapKey) / sizeof(SKeyMapping); i++)
	{
		if (cpctMapKey[i].cpcKeyID == pCpcKeyID)
		{
			return cpctMapKey[i].winKeyID;
		}
	}
	return 0;
}

u16 GetCpcKey(u16 pVKeyID)
{
	for (int i = 0; i < sizeof(cpctMapKey) / sizeof(SKeyMapping); i++)
	{
		if (cpctMapKey[i].winKeyID == pVKeyID)
		{
			return cpctMapKey[i].cpcKeyID;
		}
	}
	return 0;
}




COLORREF GetColorFW(int pFW)
{
	return _palette[pFW].rgb;
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

void DisplayFontM0(u8* pVideo, u8 fgPen, u8 bgPen, char pChara)
{
	int index = FindCharaIndex(pChara);
	int fontx = (index % FONT_NB_LINE-1);
	int fonty = (FONT_NB_COL - index / FONT_NB_LINE) * FONT_SIZE - 1;

	const u8* pixChara = fonts + fonty * FONT_NB_LINE + fontx;

	/** 2 pixels per Byte */
	for (int yi = 0; yi < FONT_SIZE; yi++)
	{
		u8 val = *pixChara;

		/* 1111 0000 */
		u8 pix3 =	((val & 0b00000001) << 0) == 0 ? bgPen : fgPen;
		pix3 |=		((val & 0b00000010) << 3) == 0 ? bgPen << 4 : fgPen << 4;
		
		u8 pix2 =	((val & 0b00000100) >> 2) == 0 ? bgPen : fgPen;
		pix2 |=		((val & 0b00001000) << 1) == 0 ? bgPen << 4 : fgPen << 4;
		
		u8 pix1 =	((val & 0b00010000) >> 4) == 0 ? bgPen : fgPen;
		pix1 |=		((val & 0b00100000) >> 1) == 0 ? bgPen << 4 : fgPen << 4;
		
		u8 pix0 =	((val & 0b01000000) >> 6) == 0 ? bgPen : fgPen;
		pix0 |=		((val & 0b10000000) >> 3) == 0 ? bgPen << 4 : fgPen << 4;

		*pVideo++ = pix0;
		*pVideo++ = pix1;
		*pVideo++ = pix2;
		*pVideo++ = pix3;

		pixChara -= FONT_NB_LINE;
		pVideo += (CPC_SCR_CX_BYTES - 4);
	}

	MsgLoop();
}


void DisplayFontM1(u8* pVideo, u8 fgPen, u8 bgPen, char pChara)
{
	int index = FindCharaIndex(pChara);
	int fontx = (index % FONT_NB_LINE - 1);
	int fonty = (FONT_NB_COL - index / FONT_NB_LINE) * FONT_SIZE - 1;

	const u8* pixChara = fonts + fonty * FONT_NB_LINE + fontx;

	/** 4 pixels per Byte */
	for (int yi = 0; yi < FONT_SIZE; yi++)
	{
		u8 val = *pixChara;

		/* 33 22 11 00 */
		u8 pix0 =	((val & 0b00000001) << 0) == 0 ? bgPen : fgPen;
		pix0 |=		((val & 0b00000010) << 1) == 0 ? bgPen << 2 : fgPen << 2;
		pix0 |=		((val & 0b00000100) << 2) == 0 ? bgPen << 4 : fgPen << 4;
		pix0 |=		((val & 0b00001000) << 3) == 0 ? bgPen << 6 : fgPen << 6;

		u8 pix1 =	((val & 0b00010000) >> 4) == 0 ? bgPen : fgPen;
		pix1 |=		((val & 0b00100000) >> 3) == 0 ? bgPen << 2 : fgPen << 2;
		pix1 |=		((val & 0b01000000) >> 2) == 0 ? bgPen << 4 : fgPen << 4;
		pix1 |=		((val & 0b10000000) >> 1) == 0 ? bgPen << 6 : fgPen << 6;

		*pVideo++ = pix0;
		*pVideo++ = pix1;

		pixChara -= FONT_NB_LINE;
		pVideo += (CPC_SCR_CX_BYTES - 2);
	}

	MsgLoop();
}

void DisplayFontM2(u8* pVideo, u8 pPen, char pChara)
{
	int index = FindCharaIndex(pChara);
	int fontx = (index % FONT_NB_LINE - 1);
	int fonty = (FONT_NB_COL - index / FONT_NB_LINE) * FONT_SIZE - 1;

	const u8* pixChara = fonts + fonty * FONT_NB_LINE + fontx;

	/** 8 pixels per Byte */
	for (int yi = 0; yi < FONT_SIZE; yi++)
	{
		u8 val = *pixChara;

		u8 pix0 =	((val & 0b00000001) == 0) ? 0 : pPen << 0;
		pix0 |=		((val & 0b00000010) == 0) ? 0 : pPen << 1;
		pix0 |=		((val & 0b00000100) == 0) ? 0 : pPen << 2;
		pix0 |=		((val & 0b00001000) == 0) ? 0 : pPen << 3;
		pix0 |=		((val & 0b00010000) == 0) ? 0 : pPen << 4;
		pix0 |=		((val & 0b00100000) == 0) ? 0 : pPen << 5;
		pix0 |=		((val & 0b01000000) == 0) ? 0 : pPen << 6;
		pix0 |=		((val & 0b10000000) == 0) ? 0 : pPen << 7;

		*pVideo++ = pix0;
		pixChara -= FONT_NB_LINE;
		pVideo += (CPC_SCR_CX_BYTES - 1);
	}

	MsgLoop();
}

void DrawString(void* string, void* video_memory, u8 fg_pen, u8 bg_pen, int pMode)
{
	if (IsCpcMem(video_memory))
		video_memory = GetVideoBufferFromAddress((int)video_memory);

	u8* str = (u8*)string;
	u8* video = (u8*)video_memory;

	for (UCHAR i = 0; i < strlen(str); i++)
	{
		if (str[i] != ' ')
		{
			switch (pMode)
			{
				case MODE_0	: DisplayFontM0(video + i * 4, fg_pen, bg_pen, str[i]);	break;
				case MODE_1 : DisplayFontM1(video + i * 2, fg_pen, bg_pen, str[i]);	break;
				case MODE_2 : DisplayFontM2(video + i * 1, bg_pen, str[i]);	break;
			}
		}
	}
}

void DrawSprite(void *sprite, void *memory, int cx, int cy, BOOL pMasked)
{
	UCHAR* video = (UCHAR*)memory;
	
	if (pMasked)
	{
		WORD* pix = (WORD*)sprite;
		for (int yi = 0; yi < cy; yi++)
		{
			for (int xi = 0; xi < cx; xi++)
			{
				UCHAR mask = DecodePixel((UCHAR)(*pix));
				UCHAR sprite = DecodePixel((UCHAR)(*pix >> 8));
				*video = *video ^ sprite;
				*video = *video & mask;
				*video = *video ^ sprite;

				video++;
				pix++;
			}
			video += (CPC_SCR_CX_BYTES - cx);
		}
	}
	else
	{
		UCHAR* pix = (UCHAR*)sprite;
		for (int yi = 0; yi < cy; yi++)
		{
			for (int xi = 0; xi < cx; xi++)
			{
				*video = DecodePixel(*pix);
				video++;
				pix++;
			}
			video += (CPC_SCR_CX_BYTES - cx);
		}
	}

	MsgLoop();
}

UCHAR* GetRenderingBuffer()
{
	UCHAR *buff = GetCurVideoBuffer();

	/** Convert mode 0 to mode 1 4bits */
	if (_amstrad._mode == 1)
	{
		UCHAR* buffMode1 = _amstrad._mode1Video;
		
		int i = 0, j = 0;
		while (i < 0x4000)
		{
			UCHAR valPix = buff[i++];

			UCHAR pix3 = (valPix & 0b00000011);
			UCHAR pix2 = (valPix & 0b00001100) << 2;
			UCHAR pix1 = (valPix & 0b00110000) >> 4;
			UCHAR pix0 = (valPix & 0b11000000) >> 2;

			valPix = buff[i++];

			UCHAR pix7 = (valPix & 0b00000011);
			UCHAR pix6 = (valPix & 0b00001100) << 2;
			UCHAR pix5 = (valPix & 0b00110000) >> 4;
			UCHAR pix4 = (valPix & 0b11000000) >> 2;

			buffMode1[j++] = pix5 | pix4;
			buffMode1[j++] = pix7 | pix6;
			buffMode1[j++] = pix1 | pix0;
			buffMode1[j++] = pix3 | pix2;
		}

		buff = buffMode1;
	}

	return buff;
}


void StartCPC()
{
	ZeroMemory(&_amstrad, sizeof(_amstrad));
	for (int i = 0; i < NB_COLORS; i++)
		_amstrad._curPal[i] = _palette[i].hw;

	for (int i = 0; i < 4; i++)
		memset(&_amstrad._memVideo[i], 0, CPC_SCR_CX_BYTES * CPC_SCR_CY_LINE);

	_amstrad._mode = 1;
	_amstrad._curPal[0] = HW_BRIGHT_BLUE;
	_amstrad._curPal[1] = HW_BRIGHT_YELLOW;
	_amstrad._curPal[BORDER_COLOR] = HW_BRIGHT_BLUE;

	_amstrad._currentPage = cpct_pageC0;

	_widthVideo = ALIGNED_DWORD(FULL_SCREEN_CX);

	StartInterrupt();
}

void ScanKeyboard()
{
	ZeroMemory(cpct_keyboardStatusBuffer, sizeof(cpct_keyboardStatusBuffer));
	_curKey = FALSE;
	MsgLoop();
}

void CPCTeleraWin()
{
	if (!_isStarted)
	{
		_isStarted = TRUE;

		StartCPC();

		CreateWindowApp();
		MsgLoop();
	}
}
