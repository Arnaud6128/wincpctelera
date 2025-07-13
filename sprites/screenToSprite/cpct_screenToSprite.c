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

extern void wincpct_computeCrossBoundary(u16* videoAddress, u8 cx);
extern u16 wincpct_getCpcMemAddress(void* address);

void cpct_getScreenToSprite(u8* memory, u8* sprite, u8 width, u8 height)
{
	u16 videoAddress = (u16)memory;

  	for (int yi = 0; yi < height; yi++)
	{
		u8* ptrVideo = &gAmstrad._memCPC[videoAddress];
		for (int xi = 0; xi < width; xi++)
		{
			*sprite++ = *ptrVideo++;
			videoAddress++;
		}
		
		wincpct_computeCrossBoundary(&videoAddress, width);
	}
}