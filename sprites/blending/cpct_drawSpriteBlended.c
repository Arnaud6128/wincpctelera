//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
//  Copyright (C) 2018 Arnaud Bouche
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

extern int wincpct_getCpcMem(int address);
extern u8* wincpct_getPCMem(int address);
extern void wincpct_computeCrossBoundary(u16* videoAddress, u8 cx);

static CPCT_BlendMode _blendMode = CPCT_BLEND_XOR;
static int _carry;

void cpct_setBlendMode(CPCT_BlendMode mode)
{
	_blendMode = mode;
}

void cpct_drawSpriteBlended(void* memory, u8 height, u8 width, void* sprite)
{
	u16 videoAddress = wincpct_getCpcMem((int)memory);
	u8* src = (u8*)wincpct_getMemory(sprite);

	for (int y = 0; y < height; y++)
	{
		u8* ptrVideo = wincpct_getPCMem(videoAddress);
		for (int x = 0; x < width; x++)
		{
			u8 pixSrc = *src;

			switch (_blendMode)
			{
				case CPCT_BLEND_XOR:
				{
					*ptrVideo ^= pixSrc;
				}
				break;

				case CPCT_BLEND_AND:
				{
					*ptrVideo &= pixSrc;
				}
				break;

				case CPCT_BLEND_OR:
				{
					*ptrVideo |= pixSrc;
				}
				break;

				case CPCT_BLEND_ADD:
				{
					int dstWithCarry = *ptrVideo + pixSrc;
					*ptrVideo = (u8)dstWithCarry;
					if (dstWithCarry > 0xFF)
						_carry = dstWithCarry - 0xFF;
				}
				break;

				case CPCT_BLEND_ADC:
				{
					int dstWithCarry = *ptrVideo + pixSrc + _carry;
					*ptrVideo = (u8)dstWithCarry;
					if (dstWithCarry > 0xFF)
						_carry = dstWithCarry - 0xFF;
				}
				break;

				case CPCT_BLEND_SUB:
				{
					int dstWithCarry = *ptrVideo - pixSrc;
					*ptrVideo = (u8)dstWithCarry;
					if (dstWithCarry < 0)
						_carry = -dstWithCarry;
				}
				break;

				case CPCT_BLEND_SBC:
				{
					int dstWithCarry = *ptrVideo - pixSrc + _carry;
					*ptrVideo = (u8)dstWithCarry;
					if (dstWithCarry < 0)
						_carry = -dstWithCarry;
				}
				break;

				case CPCT_BLEND_LDI:
				{
					*ptrVideo = pixSrc;
				}
				break;

				case CPCT_BLEND_NOP:
				{
					*ptrVideo = *ptrVideo;
				}
				break;
			}

			ptrVideo++;
			src++;
			videoAddress++;
		}

		wincpct_computeCrossBoundary(&videoAddress, width);
	}
}