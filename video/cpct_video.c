//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
//  Copyright (C) 2016 Arnaud Bouche
//  Copyright (C) 2016 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//-------------------------------------------------------------------------------

#include "winCpctelera.h"

const SCPCPalette gCpcPalette[] =
{
	0, HW_BLACK, RGB(0,0,0),
	1, HW_BLUE, RGB(0,0,128),
	1, HW_BLUE + 0x0C, RGB(0,0,128),
	2, HW_BRIGHT_BLUE, RGB(0,0,255),
	3, HW_RED, RGB(128,0,0),
	4, HW_MAGENTA, RGB(128,0,128),
	5, HW_MAUVE, RGB(128,0,255),
	6, HW_BRIGHT_RED, RGB(255,0,0),
	7,HW_PURPLE, RGB(255,0,128),
	7, HW_PURPLE + 0x03, RGB(255,0,128),
	8, HW_BRIGHT_MAGENTA, RGB(255,0,255),
	9, HW_GREEN, RGB(0,128,0),
	10, HW_CYAN, RGB(0,128,128),
	11, HW_SKY_BLUE, RGB(0,128,255),
	12, HW_YELLOW, RGB(128,128,0),
	13 ,HW_WHITE, RGB(128,128,128),
	13, HW_WHITE + 0x01, RGB(128,128,128),
	14, HW_PASTEL_BLUE, RGB(128,128,255),
	15, HW_ORANGE, RGB(255,128,0),
	16, HW_PINK, RGB(255,128,128),
	17, HW_PASTEL_MAGENTA, RGB(255,128,255),
	18, HW_BRIGHT_GREEN, RGB(0,255,0),
	19, HW_SEA_GREEN, RGB(0,255,128),
	19, HW_SEA_GREEN + 0x0F, RGB(0,255,128),
	20, HW_BRIGHT_CYAN, RGB(0,255,255),
	21, HW_LIME, RGB(128,255,0),
	22, HW_PASTEL_GREEN, RGB(128,255,128),
	23, HW_PASTEL_CYAN, RGB(128,255,255),
	24, HW_BRIGHT_YELLOW, RGB(255,255,0),
	25, HW_PASTEL_YELLOW, RGB(255,255,128),
	25, HW_PASTEL_YELLOW + 0x06, RGB(255,255,128),
	26, HW_BRIGHT_WHITE, RGB(255,255,255)
};

void cpct_fw2hw(void *fw_colour_array, u16 size)
{
	u8* fwPal = (u8*)fw_colour_array;
	for (u16 i = 0; i < size; i++)
		fwPal[i] = cpct_getHWColour(fwPal[i]);
}

void cpct_setPalette(u8* ink_array, u16 ink_array_size)
{
	memcpy(gAmstrad._curVideoConf.gCpcPalette, ink_array, ink_array_size);
}

void cpct_setPALColour(u8 pen, u8 hw_ink)
{
	gAmstrad._curVideoConf.gCpcPalette[pen] = hw_ink;
}

u8 cpct_getHWColour(u16 pFW)
{
	for (u16 i = 0; i < sizeof(gCpcPalette) / sizeof(SCPCPalette); i++)
	{
		if (gCpcPalette[i].fw == pFW)
			return gCpcPalette[i].hw;
	}
	return gCpcPalette[0].hw;
}

void cpct_waitVSYNC()
{	
	wincpct_msgLoop();
	wincpct_waitVSync();
}

u16 cpct_count2VSYNC()
{
	return gAmstrad._internalTimer / INTERRUPT_PER_VBL * REFRESH_MS * 1000;
}

void cpct_clearScreen(u8 colour_pattern)
{
	memset(wincpct_getVideoBufferFromAddress(0xC000), colour_pattern, 0x4000);
	wincpct_wait(98); // This function takes 98331 microseconds to fill the screen cf. CPCTelera manual
}

void cpct_setVideoMode(u8 videoMode)
{
	gAmstrad._curVideoConf._videoMode = videoMode;
}

void cpct_setVideoMemoryPage(u8 page_6LSb)
{
	gAmstrad._currentPage = page_6LSb;
}

void cpct_setVideoMemoryOffset(u8 offset)
{
	gAmstrad._memOffset = offset;
}

u8* cpct_getScreenPtr(void* screen_start, u8 x, u8 y)
{
	u8* memory = wincpct_getVideoBufferFromAddress((int)screen_start);
	return memory + y * CPC_SCR_CX_BYTES + x;
}

void wincpct_setPalette(int i, u8 pHW)
{
	gAmstrad._curVideoConf.gCpcPalette[i] = pHW;
}

DWORD wincpct_getColorHW(int pHW)
{
	if (pHW >= 0x40)
		pHW -= 0x40;

	for (int i = 0; i < sizeof(gCpcPalette) / sizeof(SCPCPalette); i++)
	{
		if (gCpcPalette[i].hw == pHW)
			return gCpcPalette[i].rgb;
	}
	return gCpcPalette[0].rgb;
}

DWORD wincpct_getColorFW(int pFW)
{
	return gCpcPalette[pFW].rgb;
}

int wincpct_getVideoArea(int pScreenAddr)
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

static int wincpct_convertScreenAddress(int pScreenAddr)
{
	int videoAddress = wincpct_getVideoArea(pScreenAddr);
	int address = pScreenAddr - videoAddress;

	int line = (address / 0x7D0);
	int lineChar = (address - line * 0x7D0) / 0x50;
	int bytes = (address - line * 0x7D0) % 0x50;

	int buffAddress = (lineChar * 8 + line) * CPC_SCR_CX_BYTES + bytes;

	return videoAddress + buffAddress;
}

u8* wincpct_getVideoBufferFromAddress(int pScreenAddr)
{
	if (wincpct_isCpcMem((void*)pScreenAddr))
	{
		int address = wincpct_convertScreenAddress(pScreenAddr);
		if (address == -1)
			return NULL;

		return gAmstrad._memCPC + address;
	}
	else
		return (u8*)pScreenAddr;
}

int wincpct_getPageAddress(int page)
{
	switch(page)
	{
	case cpct_page00:
		return 0x0000;
	case cpct_page40:
		return 0x4000;
	case cpct_page80:
		return 0x8000;
	case cpct_pageC0:
	default:
		return 0xC000;
	}
}

u8* wincpct_getVideoBufferFromPage(int pPage)
{
	return gAmstrad._memCPC + wincpct_getPageAddress(pPage);
}

u8* wincpct_getCurVideoBuffer()
{
	return wincpct_getVideoBufferFromPage(gAmstrad._currentPage);
}

/*
*	Convert pixel from CPC format to PC format
*	ex mode 0 : 0a2c 1b3d -> 3210 dcba
*	cf. cpct_px2byteM0 and cpct_px2byteM1
*/
u8 wincpct_convPixSpriteCPCtoPC(u8 pix)
{
	if (wincpct_getCurrentVideoMode() == MODE_0)
	{
		u8 pix0 = (pix & 0x80) >> 7;
		u8 pixa = (pix & 0x40) >> 6;
		u8 pix2 = (pix & 0x20) >> 5;
		u8 pixc = (pix & 0x10) >> 4;

		u8 pix1 = (pix & 0x08) >> 3;
		u8 pixb = (pix & 0x04) >> 2;
		u8 pix3 = (pix & 0x02) >> 1;
		u8 pixd = (pix & 0x01);

		return (pix3 << 7 | pix2 << 6 | pix1 << 5 | pix0 << 4 | pixd << 3 | pixc << 2 | pixb << 1 | pixa);
	}

	if (wincpct_getCurrentVideoMode() == MODE_1)
	{
		u8 pix0 = (pix & 0x80) >> 7;
		u8 pix2 = (pix & 0x40) >> 6;

		u8 pix4 = (pix & 0x20) >> 5;
		u8 pix6 = (pix & 0x10) >> 4;

		u8 pix1 = (pix & 0x08) >> 3;
		u8 pix3 = (pix & 0x04) >> 2;

		u8 pix5 = (pix & 0x02) >> 1;
		u8 pix7 = (pix & 0x01);

		u8 val = (pix1 << 7 | pix0 << 6 | pix3 << 5 | pix2 << 4 | pix5 << 3 | pix4 << 2 | pix7 << 1 | pix6);
		return val;
	}

	return pix;
}

u8* wincpct_getRenderingBuffer()
{
	static u8 sRenderBuffer[WIDTH_SCREEN*CPC_SCR_CY_LINE];

	u8 *buffVideo = wincpct_getCurVideoBuffer();

	if (gAmstrad._memOffset != 0)
		buffVideo = wincpct_applyLSBOffset(buffVideo);

	int i = 0, j = 0;
	u8 curVideo = wincpct_getCurrentVideoMode();
	int bytesPerInterrupt = 0x4000 / INTERRUPT_PER_VBL;
	int bytesProcess = 0;

	for (int k = 0; k < INTERRUPT_PER_VBL; k++)
	{
		bytesProcess = 0;
		while (bytesProcess++ < bytesPerInterrupt)
		{
			/* Convert color depth 4bpp to 8bpp */
			if (curVideo == MODE_0)
			{
				u8 valPix = buffVideo[i++];

				u8 pix0 = valPix & 0x0F;
				u8 pix1 = valPix >> 4;

				if (pix0 != 0 || pix1 != 0)
					pix1 = pix1;

				sRenderBuffer[j++] = pix1;
				sRenderBuffer[j++] = pix1;
				sRenderBuffer[j++] = pix1;
				sRenderBuffer[j++] = pix1;

				sRenderBuffer[j++] = pix0;
				sRenderBuffer[j++] = pix0;
				sRenderBuffer[j++] = pix0;
				sRenderBuffer[j++] = pix0;
			}
			/* Convert color depth 2bpp to 8bpp */
			else if (curVideo == MODE_1)
			{
				u8 valPix = buffVideo[i++];

				u8 pix0 = (valPix & 0b00000011);
				u8 pix1 = (valPix & 0b00001100) >> 2;
				u8 pix2 = (valPix & 0b00110000) >> 4;
				u8 pix3 = (valPix & 0b11000000) >> 6;

				sRenderBuffer[j++] = pix3;
				sRenderBuffer[j++] = pix3;

				sRenderBuffer[j++] = pix2;
				sRenderBuffer[j++] = pix2;

				sRenderBuffer[j++] = pix1;
				sRenderBuffer[j++] = pix1;

				sRenderBuffer[j++] = pix0;
				sRenderBuffer[j++] = pix0;

			}
			/* Convert color depth 2bpp to 8bpp */
			else if (curVideo == MODE_2)
			{
				u8 valPix = buffVideo[i++];

				u8 pix0 = (valPix & 0b00000001);
				u8 pix1 = (valPix & 0b00000010) >> 1;
				u8 pix2 = (valPix & 0b00000100) >> 2;
				u8 pix3 = (valPix & 0b00001000) >> 3;
				u8 pix4 = (valPix & 0b00010000) >> 4;
				u8 pix5 = (valPix & 0b00100000) >> 5;
				u8 pix6 = (valPix & 0b01000000) >> 6;
				u8 pix7 = (valPix & 0b10000000) >> 7;

				sRenderBuffer[j++] = pix7;
				sRenderBuffer[j++] = pix6;
				sRenderBuffer[j++] = pix5;
				sRenderBuffer[j++] = pix4;
				sRenderBuffer[j++] = pix3;
				sRenderBuffer[j++] = pix2;
				sRenderBuffer[j++] = pix1;
				sRenderBuffer[j++] = pix0;
			}
		}
	}

	return sRenderBuffer;
}

/* Simulate CRCT R13 offset */
static u8* wincpct_applyLSBOffset(u8* buffVideo)
{
	static u8 SOffsetVideo[0x4000];

	int offset = gAmstrad._memOffset * 2;
	int cxLine = CPC_SCR_CX_BYTES;
	u8* destVideo = SOffsetVideo;

	int dstIndex = 0x4000 - offset;
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
