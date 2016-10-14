#include "winCpctelera.h"

static u8* ApplyLSBOffset(u8* buffVideo);

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
	HW_CYAN, RGB(0,128,128),
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

void cpct_fw2hw(void *fw_colour_array, u16 size)
{
	u8* fwPal = (u8*)fw_colour_array;
	for (u16 i = 0; i < size; i++)
		fwPal[i] = cpct_getHWColour(fwPal[i]);
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

u8 cpct_getHWColour(u16 pFW)
{
	return _palette[pFW].hw;
}

void cpct_waitVSYNC()
{
	Sleep(10);
	_amstrad._internalTimer = 0;
	MsgLoop();
}

u16 cpct_count2VSYNC()
{
	return 0;
}

void cpct_clearScreen(u8 colour_pattern)
{
	memset(GetVideoBufferFromAddress(0xC000), colour_pattern, 0x4000);
}

void cpct_setVideoMode(u8 videoMode)
{
	_amstrad._mode = videoMode;
}

void cpct_setVideoMemoryPage(u8 page_6LSb)
{
	_amstrad._currentPage = page_6LSb;
}

void cpct_setVideoMemoryOffset(u8 offset)
{
	_amstrad._memOffset = offset;
}

u8* cpct_getScreenPtr(void* screen_start, u8 x, u8 y)
{
	u8* memory = GetVideoBufferFromAddress((int)screen_start);
	return memory + y * CPC_SCR_CX_BYTES + x;
}

void SetPalette(int i, u8 pHW)
{
	_amstrad._curPal[i] = pHW;
}

COLORREF GetColorHW(int pHW)
{
	if (pHW >= 0x40)
		pHW -= 0x40;

	for (int i = 0; i < NB_PAL_COLOR; i++)
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

int GetPixelBit()
{
	switch (_amstrad._mode)
	{
		case MODE_2: return 1;
		case MODE_1: return 4; // BMP not handle 2bits bitmap
		case MODE_0:
		default: return 4;
	}
}

int GetScreenWidth()
{
	switch (_amstrad._mode)
	{
		case MODE_2: return 640;
		case MODE_1: return 320;
		case MODE_0:
		default: return 160;
	}
}

int ConvertPixelPos(int x)
{
	switch (_amstrad._mode)
	{
		case MODE_2: return x;
		case MODE_1: return x * 2;
		case MODE_0:
		default: return x * 4;
	}
}

int GetVideoArea(int pScreenAddr)
{
	int videoAddress = 0;

	if (pScreenAddr >= 0 && pScreenAddr < 0x4000)
		videoAddress = 0x0000;
	else
		if (pScreenAddr >= 0x4000 && pScreenAddr < 0x8000)
			videoAddress = 0x4000;
		else
			if (pScreenAddr >= 0x8000 && pScreenAddr < 0xC000)
				videoAddress = 0x8000;
			else
				videoAddress = 0xC000;

	return videoAddress;
}

int ConvertScreenAddress(int pScreenAddr)
{
	int videoAddress = GetVideoArea(pScreenAddr);
	int address = pScreenAddr - videoAddress;

	int line = (address / 0x800);
	int lineChar = (address - line * 0x800) / 0x50;
	int bytes = (address - line * 0x800) % 0x50;

	int buffAddress = (lineChar * 8 + line) * CPC_SCR_CX_BYTES + bytes;

	return videoAddress + buffAddress;
}

u8* GetVideoBufferFromAddress(int pScreenAddr)
{
	if (IsCpcMem((void*)pScreenAddr))
	{
		int address = ConvertScreenAddress(pScreenAddr);
		return _amstrad._memCPC + address;
	}
	else
		return (u8*)pScreenAddr;
}

u8* GetVideoBufferFromPage(int pPage)
{
	switch (pPage)
	{
		case cpct_page00:
			return _amstrad._memCPC;
		case cpct_page40:
			return _amstrad._memCPC + 0x4000;
		case cpct_page80:
			return _amstrad._memCPC + 0x8000;
		case cpct_pageC0:
		default:
			return _amstrad._memCPC + 0xC000;
	}
}

u8* GetCurVideoBuffer()
{
	return GetVideoBufferFromPage(_amstrad._currentPage);
}

u8 DecodePixel(u8 pPix)
{
	if (pPix == 0x00 || pPix == 0xFF)
		return pPix;

	if (_amstrad._mode == MODE_0)
	{
		u8 pix0 = (pPix & 0x80) >> 7;
		u8 pixa = (pPix & 0x40) >> 6;
		u8 pix2 = (pPix & 0x20) >> 5;
		u8 pixc = (pPix & 0x10) >> 4;

		u8 pix1 = (pPix & 0x08) >> 3;
		u8 pixb = (pPix & 0x04) >> 2;
		u8 pix3 = (pPix & 0x02) >> 1;
		u8 pixd = (pPix & 0x01);

		return (pix3 << 7 | pix2 << 6 | pix1 << 5 | pix0 << 4 | pixd << 3 | pixc << 2 | pixb << 1 | pixa);
	}

	if (_amstrad._mode == MODE_1)
	{
		u8 pix0 = (pPix & 0x80) >> 7;
		u8 pix2 = (pPix & 0x40) >> 6;

		u8 pix4 = (pPix & 0x20) >> 5;
		u8 pix6 = (pPix & 0x10) >> 4;

		u8 pix1 = (pPix & 0x08) >> 3;
		u8 pix3 = (pPix & 0x04) >> 2;

		u8 pix5 = (pPix & 0x02) >> 1;
		u8 pix7 = (pPix & 0x01);

		u8 val = (pix1 << 7 | pix0 << 6 | pix3 << 5 | pix2 << 4 | pix5 << 3 | pix4 << 2 | pix7 << 1 | pix6);
		return val;
	}

	return pPix;
}

u8* GetRenderingBuffer()
{
	u8 *buffVideo = GetCurVideoBuffer();

	/** Convert mode 0 to mode 1 4bits */
	if (_amstrad._mode == 1)
	{
		u8* buffMode1 = _amstrad._mode1Video;

		int i = 0, j = 0;
		while (i < 0x4000)
		{
			u8 valPix = buffVideo[i++];

			u8 pix3 = (valPix & 0b00000011);
			u8 pix2 = (valPix & 0b00001100) << 2;
			u8 pix1 = (valPix & 0b00110000) >> 4;
			u8 pix0 = (valPix & 0b11000000) >> 2;

			valPix = buffVideo[i++];

			u8 pix7 = (valPix & 0b00000011);
			u8 pix6 = (valPix & 0b00001100) << 2;
			u8 pix5 = (valPix & 0b00110000) >> 4;
			u8 pix4 = (valPix & 0b11000000) >> 2;

			buffMode1[j++] = pix1 | pix0;
			buffMode1[j++] = pix3 | pix2;
			buffMode1[j++] = pix5 | pix4;
			buffMode1[j++] = pix7 | pix6;
		}

		buffVideo = buffMode1;
	}

	if (_amstrad._memOffset != 0)
		return ApplyLSBOffset(buffVideo);

	return buffVideo;
}

static u8* ApplyLSBOffset(u8* buffVideo)
{
	int bufferSize = 0x4000;
	int offset = _amstrad._memOffset * 2;
	int cxLine = CPC_SCR_CX_BYTES;
	u8* destVideo = _amstrad._renderVideo;

	if (_amstrad._mode == MODE_1)
	{
		bufferSize *= 2;
		offset *= 2;
		cxLine *= 2;
		destVideo = _amstrad._renderMode1Video;
	}

	int dstIndex = bufferSize - offset;
	for (int i = 0; i < CPC_SCR_CY_LINE*CPC_SCR_CX_BYTES; i++)
	{
		destVideo[dstIndex++] = *buffVideo++;
		if (dstIndex > 0x3FFF)
			dstIndex = 0;
	}

	for (int x = 0; x < offset; x++)
	{
		for (int y = 7; y < CPC_SCR_CY_LINE + 7; y++)
		{
			int offsetX = CPC_SCR_CX_BYTES - x - 1;
			*(destVideo + (y - 7)*CPC_SCR_CX_BYTES + offsetX) = *(destVideo + y*CPC_SCR_CX_BYTES + offsetX);
		}
	}

	return destVideo;
}
