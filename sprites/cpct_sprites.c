//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
//  Copyright (C) 2016 Arnaud Bouche (@arnaud6128)
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

#include <winCpctelera.h>

extern u8* wincpct_getVideoBufferFromAddress(void* pScreenAddr);
extern u16 wincpct_getCpcMemAddress(void* address);
extern u8* wincpct_getPCMemPtr(u16 address);

void wincpct_computeCrossBoundary(u16* videoAddress, u8 cx)
{
	*videoAddress += (0x800 - cx);

	u8 crossBoundary = (u8)(*videoAddress >> 8);
	if ((crossBoundary & 0x38) == 0)
		*videoAddress += 0xC050;
}

void wincpct_invertComputeCrossBoundary(u16* videoAddress, u8 cx)
{
	*videoAddress -= (0x800 + cx);

	u8 crossBoundary = (u8)(*videoAddress >> 8);
	crossBoundary &= 0x38;
	crossBoundary ^= 0x38;

	if (crossBoundary == 0)
		*videoAddress += 0x3FB0;
}

static u8* _pmasktable;

u8 cpct_px2byteM0(u8 px0, u8 px1)
{
	return cpctm_px2byteM0(px0, px1);
}

u8 cpct_px2byteM1(u8 px0, u8 px1, u8 px2, u8 px3)
{
	return cpctm_px2byteM1(px0, px1, px2, px3);
}

void cpct_drawSprite(void *sprite, void* memory, u8 width, u8 height)
{
	if (width < 1 || width > 63)
		printf("In file %s line %d %s : %s", __FILE__, __LINE__, "cpct_drawSprite", "WARNING Sprite Width in bytes [1-63] (Beware, not in pixels!)");

	if (height == 0)
		printf("In file %s line %d %s : %s", __FILE__, __LINE__, "cpct_drawSprite", "WARNING Sprite Height in bytes (>0)");

	wincpct_drawSprite(sprite, memory, width, height, SPRITE_NORMAL);
}

void cpct_drawSpriteMasked(void *sprite, void* memory, u8 width, u8 height)
{
	if (width == 0)
		printf("In file %s line %d %s : %s", __FILE__, __LINE__, "cpct_drawSpriteMasked", "WARNING Sprite Width > 0");

	if (height == 0)
		printf("In file %s line %d %s : %s", __FILE__, __LINE__, "cpct_drawSpriteMasked", "WARNING Sprite Height > 0");

	wincpct_drawSprite(sprite, memory, width, height, SPRITE_MASKED);
}

void cpct_drawSolidBox(void *memory, u16 colour_pattern, u8 width, u8 height)
{
	if (width < 1 || width > 64)
		printf("In file %s line %d %s : %s", __FILE__, __LINE__, "cpct_drawSolidBox", "WARNING Sprite Width in bytes [1-64] (Beware, not in pixels!)");
	
	if (height == 0)
		printf("In file %s line %d  %s : %s", __FILE__, __LINE__, "cpct_drawSolidBox", "WARNING Sprite Height in bytes (>0)");

	u8* videoPtr = wincpct_getVideoBufferFromAddress(memory);

	u16 videoAddress = wincpct_getCpcMemAddress(videoPtr);

	for (int yi = 0; yi < height; yi++)
	{
		u8* ptrVideo = wincpct_getPCMemPtr(videoAddress);
		for (int xi = 0; xi < width; xi++)
		{
			*ptrVideo++ = (u8)colour_pattern;
			videoAddress++;
		}

		wincpct_computeCrossBoundary(&videoAddress, width);
	}
}

void cpct_drawSpriteMaskedAlignedTable(const void* psprite, void* pvideomem, u8 width, u8 height, const void* pmasktable)
{
	if (width == 0)
		printf("In file %s line %d %s : %s", __FILE__, __LINE__, "cpct_drawSpriteMaskedAlignedTable", "WARNING Sprite Width > 0");

	if (height == 0)
		printf("In file %s line %d %s : %s", __FILE__, __LINE__, "cpct_drawSpriteMaskedAlignedTable", "WARNING Sprite Height > 0");

	_pmasktable = (u8*)pmasktable;
	wincpct_drawSprite((void*)psprite, pvideomem, width, height, SPRITE_ALIGNEDTABLE);
}

void wincpct_drawSprite(void *pSprite, void *memory, int cx, int cy, u8 pSpriteMode)
{
	static u8 sSlowDown = 0;

	u8* videoMem = wincpct_getMemory(memory);
	u16 videoAddress = wincpct_getCpcMemAddress(videoMem);

	u8* sprite = (u8*)wincpct_getMemory(pSprite);

	if (pSpriteMode == SPRITE_MASKED)
	{
		WORD* pix = (WORD*)sprite;
		for (int yi = 0; yi < cy; yi++)
		{
			u8* ptrVideo = wincpct_getPCMemPtr(videoAddress);
			for (int xi = 0; xi < cx; xi++)
			{
				u8 mask = (u8)*pix;
				u8 sprite = (u8)(*pix >> 8);
				*ptrVideo = *ptrVideo ^ sprite;
				*ptrVideo = *ptrVideo & mask;
				*ptrVideo = *ptrVideo ^ sprite;

				ptrVideo++;
				pix++;
			}

			videoAddress += cx;
			wincpct_computeCrossBoundary(&videoAddress, cx);
		}
	}
	else if (pSpriteMode == SPRITE_NORMAL)
	{
		u8* pix = (u8*)sprite;
		for (int yi = 0; yi < cy; yi++)
		{
			u8* ptrVideo = wincpct_getPCMemPtr(videoAddress);
			for (int xi = 0; xi < cx; xi++)
			{
				*ptrVideo++ = *pix++;
			}

			videoAddress += cx;
			wincpct_computeCrossBoundary(&videoAddress, cx);
		}
	}
	else if (pSpriteMode == SPRITE_ALIGNEDTABLE)
	{
		u8* pix = (u8*)sprite;
		for (int yi = 0; yi < cy; yi++)
		{
			u8* ptrVideo = wincpct_getPCMemPtr(videoAddress);
			for (int xi = 0; xi < cx; xi++)
			{
				u8 pixel = *pix++; 
				u8 mask = _pmasktable[pixel];
				*ptrVideo++ = (*ptrVideo & mask) | pixel;	
			}

			videoAddress += cx;
			wincpct_computeCrossBoundary(&videoAddress, cx);
		}
	}
	else if (pSpriteMode == SPRITE_FLIP_V)
	{
		u8* pix = (u8*)sprite;
		for (int yi = 0; yi < cy; yi++)
		{
			u8* ptrVideo = wincpct_getPCMemPtr(videoAddress);
			for (int xi = 0; xi < cx; xi++)
			{
				*ptrVideo++ = *pix++;
			}

			videoAddress += cx;
			wincpct_invertComputeCrossBoundary(&videoAddress, cx);
		}
	}
	else if (pSpriteMode == SPRITE_ZIGZAG)
	{
		u8 indexZigzag[] = { 0, 1, 3, 2, 7, 6, 4, 5 };
		
		for (int yi = 0; yi < cy; yi++)
		{
			u8* ptrVideo = wincpct_getPCMemPtr(videoAddress);
			u8 zigzag = indexZigzag[yi];
			if ((zigzag % 2) == 0)
			{
				u8* pix = &sprite[zigzag * cx];
				for (int xi = 0; xi < cx; xi++)
				{
					*ptrVideo++ = *pix++;
				}
			}
			else
			{
				u8* pix = &sprite[(zigzag + 1) * cx - 1];
				for (int xi = 0; xi < cx; xi++)
				{
					*ptrVideo++ = *pix--;
				}
			}
			videoAddress += cx;
			wincpct_computeCrossBoundary(&videoAddress, cx);
		}
	}

	if ((++sSlowDown % 8) == 0)
		WinCpcTelera_Wait(1);
}