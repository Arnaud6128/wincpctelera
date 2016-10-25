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

void DrawString(void* string, void* video_memory, u8 fg_pen, u8 bg_pen, int pMode);
void DisplayFontM0(u8* pVideo, u8 fgPen, u8 bgPen, char pChara);
void DisplayFontM1(u8* pVideo, u8 fgPen, u8 bgPen, char pChara);
void DisplayFontM2(u8* pVideo, u8 pPen, char pChara);

#define FONT_SIZE		8
#define FONT_NB_LINE	32
#define FONT_NB_COL		3
#define FONT_COL		(FONT_NB_COL*FONT_SIZE)

/** Bottom - Top */
const u8 fonts[] =
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

int FindCharaIndex(char pChara)
{
	for (int i = 0; i < sizeof(charaMapBitmap); i++)
	{
		if (charaMapBitmap[i] == pChara)
			return i;
	}
	return 0;
}

void cpct_drawCharM0(void* video_memory, u8 fg_pen, u8 bg_pen, u8 ascii)
{
	if (IsCpcMem(video_memory))
		video_memory = GetVideoBufferFromAddress((int)video_memory);

	DisplayFontM0(video_memory, fg_pen, bg_pen, ascii);
}

void cpct_drawStringM0(void* string, void* video_memory, u8 fg_pen, u8 bg_pen)
{
	DrawString(string, video_memory, fg_pen, bg_pen, 0);
}

void cpct_drawCharM1(void* video_memory, u8 fg_pen, u8 bg_pen, u8 ascii)
{
	if (IsCpcMem(video_memory))
		video_memory = GetVideoBufferFromAddress((int)video_memory);

	DisplayFontM1(video_memory, fg_pen, bg_pen, ascii);
}

void cpct_drawCharM1_f(void* video_memory, u8 fg_pen, u8 bg_pen, u8 ascii)
{
	cpct_drawCharM1(video_memory, fg_pen, bg_pen, ascii);
}

void cpct_drawStringM1(void* string, void* video_memory, u8 fg_pen, u8 bg_pen)
{
	DrawString(string, video_memory, fg_pen, bg_pen, 1);
}

void cpct_drawStringM1_f(void* string, void* video_memory, u8 fg_pen, u8 bg_pen)
{
	DrawString(string, video_memory, fg_pen, bg_pen, 1);
}

void cpct_drawCharM2(void* video_memory, u8 pen, u8 ascii)
{
	if (IsCpcMem(video_memory))
		video_memory = GetVideoBufferFromAddress((int)video_memory);

	DisplayFontM2(video_memory, pen, ascii);
}

void cpct_drawStringM2(void* string, void* video_memory, u8 pen)
{
	DrawString(string, video_memory, 0, pen, 2);
}

void DisplayFontM0(u8* pVideo, u8 fgPen, u8 bgPen, char pChara)
{
	int index = FindCharaIndex(pChara);
	int fontx = (index % FONT_NB_LINE - 1);
	int fonty = (FONT_NB_COL - index / FONT_NB_LINE) * FONT_SIZE - 1;

	const u8* pixChara = fonts + fonty * FONT_NB_LINE + fontx;

	/** 2 pixels per Byte */
	for (int yi = 0; yi < FONT_SIZE; yi++)
	{
		u8 val = *pixChara;

		/* 1111 0000 */
		u8 pix3 = ((val & 0b00000001) << 0) == 0 ? bgPen : fgPen;
		pix3 |= ((val & 0b00000010) << 3) == 0 ? bgPen << 4 : fgPen << 4;

		u8 pix2 = ((val & 0b00000100) >> 2) == 0 ? bgPen : fgPen;
		pix2 |= ((val & 0b00001000) << 1) == 0 ? bgPen << 4 : fgPen << 4;

		u8 pix1 = ((val & 0b00010000) >> 4) == 0 ? bgPen : fgPen;
		pix1 |= ((val & 0b00100000) >> 1) == 0 ? bgPen << 4 : fgPen << 4;

		u8 pix0 = ((val & 0b01000000) >> 6) == 0 ? bgPen : fgPen;
		pix0 |= ((val & 0b10000000) >> 3) == 0 ? bgPen << 4 : fgPen << 4;

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
		u8 pix0 = ((val & 0b00000001) << 0) == 0 ? bgPen : fgPen;
		pix0 |= ((val & 0b00000010) << 1) == 0 ? bgPen << 2 : fgPen << 2;
		pix0 |= ((val & 0b00000100) << 2) == 0 ? bgPen << 4 : fgPen << 4;
		pix0 |= ((val & 0b00001000) << 3) == 0 ? bgPen << 6 : fgPen << 6;

		u8 pix1 = ((val & 0b00010000) >> 4) == 0 ? bgPen : fgPen;
		pix1 |= ((val & 0b00100000) >> 3) == 0 ? bgPen << 2 : fgPen << 2;
		pix1 |= ((val & 0b01000000) >> 2) == 0 ? bgPen << 4 : fgPen << 4;
		pix1 |= ((val & 0b10000000) >> 1) == 0 ? bgPen << 6 : fgPen << 6;

		*pVideo++ = pix1;
		*pVideo++ = pix0;

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

		u8 pix0 = ((val & 0b00000001) == 0) ? 0 : pPen << 0;
		pix0 |= ((val & 0b00000010) == 0) ? 0 : pPen << 1;
		pix0 |= ((val & 0b00000100) == 0) ? 0 : pPen << 2;
		pix0 |= ((val & 0b00001000) == 0) ? 0 : pPen << 3;
		pix0 |= ((val & 0b00010000) == 0) ? 0 : pPen << 4;
		pix0 |= ((val & 0b00100000) == 0) ? 0 : pPen << 5;
		pix0 |= ((val & 0b01000000) == 0) ? 0 : pPen << 6;
		pix0 |= ((val & 0b10000000) == 0) ? 0 : pPen << 7;

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

	for (u8 i = 0; i < strlen(str); i++)
	{
		if (str[i] != ' ')
		{
			switch (pMode)
			{
			case MODE_0: DisplayFontM0(video + i * 4, fg_pen, bg_pen, str[i]);	break;
			case MODE_1: DisplayFontM1(video + i * 2, fg_pen, bg_pen, str[i]);	break;
			case MODE_2: DisplayFontM2(video + i * 1, bg_pen, str[i]);	break;
			}
		}
	}

	MsgLoop();
}

