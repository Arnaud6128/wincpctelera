
//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
//  Copyright (C) 2018 Arnaud Bouche (@arnaud6128)
//  Copyright (C) 2017 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
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

extern u8* _pmasktable;

void cpct_drawToSpriteBuffer(u16 buffer_width, void* buffer, u8 width, u8 height, void* sprite)
{
	u8* buff = wincpct_getMemory(buffer);
	u8* spr = wincpct_getMemory(sprite);

	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
		{
			*buff = *spr;
			buff++;
			spr++;
		}
		buff += (buffer_width - width);
	}
}

void cpct_drawToSpriteBufferMasked(u16 buffer_width, void* buffer, u8 width, u8 height,	void* sprite)
{
	u8* buff = wincpct_getMemory(buffer);
	u16* spr = (u16*)wincpct_getMemory(sprite);

	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
		{
			u8 mask = (u8)*spr;
			u8 sprite = (u8)(*spr >> 8);
			*buff = *buff ^ sprite;
			*buff = *buff & mask;
			*buff = *buff ^ sprite;

			buff++;
			spr++;
		}
		buff += (buffer_width - width);
	}
}

void cpct_drawToSpriteBufferMaskedAlignedTable(u16 buffer_width, void* buffer, u8 width, u8 height,	void* sprite, u8* mask_table)
{
	_pmasktable = (u8*)mask_table;

	u8* buff = wincpct_getMemory(buffer);
	u8* spr = wincpct_getMemory(sprite);

	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
		{
			u8 pixel = *spr++;
			u8 mask = _pmasktable[pixel];
			*buff++ = (*buff & mask) | pixel;
		}
		buff += (buffer_width - width);
	}
}