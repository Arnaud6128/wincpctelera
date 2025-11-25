//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of WinCPCtelera: An Amstrad CPC Game Engine
//  Copyright (C) 2022 Arnaud Bouche (@Arnaud6128)
//  Copyright (C) 2022 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
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
//------------------------------------------------------------------------------

/** CPCTelera includes */
#include <cpctelera.h>

extern void wincpct_computeCrossBoundary(u16* videoAddress, u8 cx);
extern u16 wincpct_getCpcMemAddress(void* address);
extern u8* wincpct_getPCMemPtr(u16 address);

void cpct_drawSpriteClipped(u16 width_sprite_to_draw, u8* destMemory, u8 width, u8 height, u8* sprite)
{
	u8* videoMem = wincpct_getMemory(destMemory);
	u16 videoAddress = wincpct_getCpcMemAddress(videoMem);

	u8* pix = (u8*)wincpct_getMemory(sprite);
	for (int yi = 0; yi < height; yi++)
	{
		u8* ptrVideo = wincpct_getPCMemPtr(videoAddress);
		for (int xi = 0; xi < width_sprite_to_draw; xi++)
		{
			*ptrVideo++ = *pix++;
			videoAddress++;
		}

		pix += (width - width_sprite_to_draw);
		wincpct_computeCrossBoundary(&videoAddress, (u8)width_sprite_to_draw);
	}
}

void cpct_drawSpriteClippedMaskedAlignedTable(u16 width_sprite_to_draw, u8* destMemory, u8 width, u8 height, u8* sprite, u8* mask_table)
{
	u8* videoMem = wincpct_getMemory(destMemory);
	u16 videoAddress = wincpct_getCpcMemAddress(videoMem);

	u8* pix = (u8*)sprite;
	for (int yi = 0; yi < height; yi++)
	{
		u8* ptrVideo = wincpct_getPCMemPtr(videoAddress);
		for (int xi = 0; xi < width_sprite_to_draw; xi++)
		{
			u8 pixel = *pix++;
			u8 mask = mask_table[pixel];
			*ptrVideo++ = (*ptrVideo & mask) | pixel;

			videoAddress++;
		}

		pix += (width - width_sprite_to_draw);
		wincpct_computeCrossBoundary(&videoAddress, (u8)width_sprite_to_draw);
	}
}

void cpct_drawSpriteClippedMasked(u16 width_sprite_to_draw, u8* destMemory, u8 width, u8 height, u8* sprite)
{
	u8* videoMem = wincpct_getMemory(destMemory);
	u16 videoAddress = wincpct_getCpcMemAddress(videoMem);

	u16* pix = (u16*)sprite;
	for (int yi = 0; yi < height; yi++)
	{
		u8* ptrVideo = wincpct_getPCMemPtr(videoAddress);
		for (int xi = 0; xi < width_sprite_to_draw; xi++)
		{
			u8 mask = (u8)*pix;
			u8 sprite = (u8)(*pix >> 8);
			*ptrVideo = *ptrVideo ^ sprite;
			*ptrVideo = *ptrVideo & mask;
			*ptrVideo = *ptrVideo ^ sprite;

			ptrVideo++;
			pix++;
		}

		pix += (width - width_sprite_to_draw) * 2;
		wincpct_computeCrossBoundary(&videoAddress, (u8)width_sprite_to_draw);
	}
}