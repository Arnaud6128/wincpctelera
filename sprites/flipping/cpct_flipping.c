//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
//  Copyright (C) 2018 Arnaud Bouche (@arnaud6128)
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

static void wincpct_hflipByte(u8 width, u8 height, u8* sprite)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width / 2; x++)
		{
			u8* dest = sprite + width - x - 1;
			u8* src = sprite + x;

			u8 temp = *dest;
			*dest = *src;
			*src = temp;
		}
		sprite += width;
	}
}

/*
*	Convert pixel from PC format to CPC format and flip
*	ex mode 0 : 3210 dcba -> 0a2c 1b3d -> a0c2 b1d3
*	cf. cpct_px2byteM0 and cpct_px2byteM1
*/
static u8 wincpct_decodeFlipPixMode0(u8 data)
{
	u8 pix0 = (data & 0x80) >> 7;
	u8 pixa = (data & 0x40) >> 6;
	u8 pix2 = (data & 0x20) >> 5;
	u8 pixc = (data & 0x10) >> 4;
	u8 pix1 = (data & 0x08) >> 3;
	u8 pixb = (data & 0x04) >> 2;
	u8 pix3 = (data & 0x02) >> 1;
	u8 pixd = (data & 0x01);

	return (pix0 << 6 | pixa << 7 | pix2 << 4 | pixc << 5 | pix1 << 2 | pixb << 3 | pix3 << 0 | pixd << 1);
}

void cpct_hflipSpriteM0(u8 width, u8 height, void* sprite)
{
	u8* data = (u8*)sprite;
	wincpct_hflipByte(width, height, data);

	for (int i = 0; i < width*height; i++)
		*data++ = wincpct_decodeFlipPixMode0(*data);
}

void cpct_hflipSpriteM0_f(u8 width, u8 height, void* sprite)
{
	cpct_hflipSpriteM0(width, height, (u8*)sprite);
}

static u8 wincpct_decodeFlipPixMode1(u8 data)
{
	u8 pix0 = (data & 0x80) >> 7;
	u8 pix1 = (data & 0x40) >> 6;
	u8 pix2 = (data & 0x20) >> 5;
	u8 pix3 = (data & 0x10) >> 4;
	u8 pixa = (data & 0x08) >> 3;
	u8 pixb = (data & 0x04) >> 2;
	u8 pixc = (data & 0x02) >> 1;
	u8 pixd = (data & 0x01);

	return (pix3 << 7 | pix2 << 6 | pix1 << 5 | pix0 << 4 | pixd << 3 | pixc << 2 | pixb << 1 | pixa);
}

void cpct_hflipSpriteM1(u8 width, u8 height, void* sprite)
{
	u8* data = (u8*)wincpct_getMemory(sprite);
	wincpct_hflipByte(width, height, data);

	data = sprite;
	for (int i = 0; i < width*height; i++)
		*data++ = wincpct_decodeFlipPixMode1(*data);
}

void cpct_hflipSpriteM1_f(u8 width, u8 height, void* sprite)
{
	cpct_hflipSpriteM1(width, height, (u8*)sprite);
}

static u8 wincpct_decodeFlipPixMode2(u8 data)
{
	u8 pix0 = (data & 0x80) >> 7;
	u8 pix1 = (data & 0x40) >> 6;
	u8 pix2 = (data & 0x20) >> 5;
	u8 pix3 = (data & 0x10) >> 4;
	u8 pix4 = (data & 0x08) >> 3;
	u8 pix5 = (data & 0x04) >> 2;
	u8 pix6 = (data & 0x02) >> 1;
	u8 pix7 = (data & 0x01);

	return (pix7 << 7 | pix6 << 6 | pix5 << 5 | pix4 << 4 | pix3 << 3 | pix2 << 2 | pix1 << 1 | pix0 << 0);
}

void cpct_hflipSpriteM2(u8 width, u8 height, void* sprite)
{
	u8* data = (u8*)wincpct_getMemory(sprite);
	wincpct_hflipByte(width, height, data);

	data = sprite;
	for (int i = 0; i < width*height; i++)
		*data++ = wincpct_decodeFlipPixMode2(*data);
}

void cpct_hflipSpriteM2_f(u8 width, u8 height, void* sprite)
{
	cpct_hflipSpriteM2(width, height, (u8*)sprite);
}

static void wincpct_hflipByteMasked(u8 width, u8 height, u16* sprite)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width / 2; x++)
		{
			u16* dest = sprite + width - x - 1;
			u16* src = sprite + x;

			u16 temp = *dest;
			*dest = *src;
			*src = temp;
		}
		sprite += width;
	}
}

void cpct_hflipSpriteMaskedM0(u8 width, u8 height, void* sprite)
{
	u8* data = (u8*)wincpct_getMemory(sprite);
	wincpct_hflipByteMasked(width, height, sprite);

	for (int i = 0; i < width*height * 2; i++)
		*data++ = wincpct_decodeFlipPixMode0(*data);
}

void cpct_hflipSpriteMaskedM1(u8 width, u8 height, void* sprite)
{
	u8* data = (u8*)wincpct_getMemory(sprite);
	wincpct_hflipByteMasked(width, height, sprite);

	for (int i = 0; i < width*height*2; i++)
		*data++ = wincpct_decodeFlipPixMode1(*data);
}

void cpct_hflipSpriteMaskedM2(u8 width, u8 height, void* sprite)
{
	u8* data = (u8*)wincpct_getMemory(sprite);
	wincpct_hflipByteMasked(width, height, sprite);

	for (int i = 0; i < width*height * 2; i++)
		*data++ = wincpct_decodeFlipPixMode2(*data);
}