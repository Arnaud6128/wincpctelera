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

u8 _transparentColor;

u8 cpct_px2byteM0(u8 px0, u8 px1)
{
	return (px1 << 4) | (0x0F & px0);
}

u8 cpct_px2byteM1(u8 px0, u8 px1, u8 px2, u8 px3)
{
	return (px0 << 6) | (px1 << 4) | (px2 << 2) | px3;
}

void cpct_drawSprite(void *sprite, void* memory, u8 width, u8 height)
{
	wincpct_drawSprite(sprite, memory, width, height, SPRITE_NORMAL);

	wincpct_wait(2);
}

void cpct_drawSpriteMasked(void *sprite, void* memory, u8 width, u8 height)
{
	wincpct_drawSprite(sprite, memory, width, height, SPRITE_MASKED);

	wincpct_wait(2);
}

void cpct_drawSolidBox(void *memory, u8 colour_pattern, u8 width, u8 height)
{
	u8* video = (u8*)wincpct_getVideoBufferFromAddress((int)memory);

	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
		{
			*video = colour_pattern;
			video++;
		}
		video += (CPC_SCR_CX_BYTES - width);
	}

	wincpct_wait(2);
}

void cpct_drawSpriteMaskedAlignedTable(const void* psprite, void* pvideomem, u8 width, u8 height, const void* pmasktable)
{
	_transparentColor = ((u8*)pmasktable)[0];
	wincpct_drawSprite((void*)psprite, pvideomem, width, height, SPRITE_ALIGNEDTABLE);

	wincpct_wait(2);
}


void wincpct_filterPixel(u8* video, u8 pix, u8 transColor)
{
	if (wincpct_getCurrentVideoMode() == MODE_0)
	{
		if ((pix & 0x0F) >> 0 != transColor)	
			*video = (*video & 0xF0) | (0x0F & pix);

		if ((pix & 0xF0) >> 4 != transColor)
			*video = (*video & 0x0F) | (0xF0 & pix);
	}
	else
	if (wincpct_getCurrentVideoMode() == MODE_1)
	{
		if ((pix & 0x03) >> 0 != transColor)	*video = (*video & 0xFC) | (0x03 & pix);
		if ((pix & 0x0C) >> 2 != transColor)	*video = (*video & 0xF3) | (0x0C & pix);
		if ((pix & 0x30) >> 4 != transColor)	*video = (*video & 0xCF) | (0x30 & pix);
		if ((pix & 0xC0) >> 6 != transColor)	*video = (*video & 0x3F) | (0xC0 & pix);
	}
}

void wincpct_drawSprite(void *pSprite, void *memory, int cx, int cy, u8 pSpriteMode)
{
	u8* video = wincpct_getVideoBufferFromAddress((int)memory);
	u8* sprite = (u8*)wincpct_getMemory(pSprite);

	if (pSpriteMode == SPRITE_MASKED)
	{
		WORD* pix = (WORD*)sprite;
		for (int yi = 0; yi < cy; yi++)
		{
			for (int xi = 0; xi < cx; xi++)
			{
				u8 mask = wincpct_convPixSpriteCPCtoPC((u8)(*pix));
				u8 sprite = wincpct_convPixSpriteCPCtoPC((u8)(*pix >> 8));
				*video = *video ^ sprite;
				*video = *video & mask;
				*video = *video ^ sprite;

				video++;
				pix++;
			}
			video += (CPC_SCR_CX_BYTES - cx);
		}
	}
	else if (pSpriteMode == SPRITE_NORMAL)
	{
		u8* pix = (u8*)sprite;
		for (int yi = 0; yi < cy; yi++)
		{
			for (int xi = 0; xi < cx; xi++)
			{
				*video = wincpct_convPixSpriteCPCtoPC(*pix);
				video++;
				pix++;
			}
			video += (CPC_SCR_CX_BYTES - cx);
		}
	}
	else if (pSpriteMode == SPRITE_ALIGNEDTABLE)
	{
		u8* pix = (u8*)sprite;
		for (int yi = 0; yi < cy; yi++)
		{
			for (int xi = 0; xi < cx; xi++)
			{
				u8 pixel = wincpct_convPixSpriteCPCtoPC(*pix);

				/** Filter only index 0 in cpctelera */
				if (_transparentColor == 0)
					wincpct_filterPixel(video, pixel, _transparentColor);

				video++;
 				pix++;
			}
			video += (CPC_SCR_CX_BYTES - cx);
		}
	}
}