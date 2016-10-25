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

#include <winCpctelera.h>

static CPCT_BlendMode _blendMode = CPCT_BLEND_XOR;
static int _carry;
static u8 _transparentColor;

void cpct_setBlendMode(CPCT_BlendMode mode)
{
	_blendMode = mode;
}

void cpct_drawSpriteBlended(void* memory, u8 height, u8 width, void* sprite)
{
	u8* dst = (u8*)memory;
	u8* src = (u8*)sprite;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			u8 pixSrc = ConvPixCPCtoPC(*src);

			switch (_blendMode)
			{
				case CPCT_BLEND_XOR:
				{
					*dst ^= pixSrc;
				}
				break;

				case CPCT_BLEND_AND:
				{
					*dst &= pixSrc;
				}
				break;

				case CPCT_BLEND_OR:
				{
					*dst |= pixSrc;
				}
				break;

				case CPCT_BLEND_ADD:
				{
					int dstWithCarry = *dst + pixSrc;
					*dst = (u8)dstWithCarry;
					if (dstWithCarry > 0xFF)
						_carry = dstWithCarry - 0xFF;
				}
				break;

				case CPCT_BLEND_ADC:
				{
					int dstWithCarry = *dst + pixSrc + _carry;
					*dst = (u8)dstWithCarry;
					if (dstWithCarry > 0xFF)
						_carry = dstWithCarry - 0xFF;
				}
				break;

				case CPCT_BLEND_SUB:
				{
					int dstWithCarry = *dst - pixSrc;
					*dst = (u8)dstWithCarry;
					if (dstWithCarry < 0)
						_carry = -dstWithCarry;
				}
				break;

				case CPCT_BLEND_SBC:
				{
					int dstWithCarry = *dst - pixSrc + _carry;
					*dst = (u8)dstWithCarry;
					if (dstWithCarry < 0)
						_carry = -dstWithCarry;
				}
				break;

				case CPCT_BLEND_LDI:
				{
					*dst = pixSrc;
				}
				break;

				case CPCT_BLEND_NOP:
				{
					*dst = *dst;
				}
				break;
			}

			dst++;
			src++;
		}

		dst += CPC_SCR_CX_BYTES - width;
	}

	Sleep(1);
}


void cpct_drawTileAligned2x4(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 2, 4);
}

void cpct_drawTileAligned2x4_f(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 2, 4);
}

void cpct_drawTileAligned2x8(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 2, 8);
}

void cpct_drawTileAligned2x8_f(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 2, 8);
}

void cpct_drawTileAligned4x4(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 4, 4);
}

void cpct_drawTileAligned4x8(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 4, 8);
}

void cpct_drawTileAligned4x8_f(void* sprite, void* memory)
{
	cpct_drawSprite(sprite, memory, 4, 8);
}

/*
*	Convert pixel from CPC format to PC format
*	ex mode 0 : 3210 dcba -> 0a2c 1b3d
*	cf. cpct_px2byteM0 and cpct_px2byteM1
*/
u8 ConvPixPCtoCPC(u8 pPix, u8 pMode)
{
	if (pMode == MODE_0)
	{
		u8 pix3 = (pPix & 0x80) >> 7;
		u8 pix2 = (pPix & 0x40) >> 6;
		u8 pix1 = (pPix & 0x20) >> 5;
		u8 pix0 = (pPix & 0x10) >> 4;

		u8 pixd = (pPix & 0x08) >> 3;
		u8 pixc = (pPix & 0x04) >> 2;
		u8 pixb = (pPix & 0x02) >> 1;
		u8 pixa = (pPix & 0x01);

		return (pix0 << 7 | pixa << 6 | pix2 << 5 | pixc << 4 | pix1 << 3 | pixb << 2 | pix3 << 1 | pixd);
	}

	if (pMode == MODE_1)
	{
		u8 pix1 = (pPix & 0x80) >> 7;
		u8 pix0 = (pPix & 0x40) >> 6;

		u8 pix3 = (pPix & 0x20) >> 5;
		u8 pix2 = (pPix & 0x10) >> 4;

		u8 pix5 = (pPix & 0x08) >> 3;
		u8 pix4 = (pPix & 0x04) >> 2;

		u8 pix7 = (pPix & 0x02) >> 1;
		u8 pix6 = (pPix & 0x01);

		u8 val = (pix0 << 7 | pix2 << 6 | pix4 << 5 | pix6 << 4 | pix1 << 3 | pix3 << 2 | pix5 << 1 | pix7);
		return val;
	}

	return pPix;
}

u8 cpct_px2byteM0(u8 px0, u8 px1)
{
	u8 pix = (px1 << 4) | (0x0F & px0);
	return ConvPixPCtoCPC(pix, MODE_0);
}

u8 cpct_px2byteM1(u8 px0, u8 px1, u8 px2, u8 px3)
{
	return (px3 << 6) | (px2 << 4) | (px1 << 2) | px0;
}

void cpct_drawSprite(void *sprite, void* memory, u8 width, u8 height)
{
	if (IsCpcMem(memory))
		memory = GetVideoBufferFromAddress((int)memory);

	DrawSprite(sprite, memory, width, height, SPRITE_NORMAL);

	Sleep(1);
}

void cpct_drawSpriteMasked(void *sprite, void* memory, u8 width, u8 height)
{
	DrawSprite(sprite, memory, width, height, SPRITE_MASKED);

	Sleep(1);
}

void cpct_drawSolidBox(void *memory, u8 colour_pattern, u8 width, u8 height)
{
	u8 pix = ConvPixCPCtoPC(colour_pattern);

	if (IsCpcMem(memory))
		memory = GetVideoBufferFromAddress((int)memory);

	u8* video = (u8*)memory;

	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
		{
			*video = pix;
			video++;
		}
		video += (CPC_SCR_CX_BYTES - width);
	}

	Sleep(1);
}

void cpct_drawSpriteMaskedAlignedTable(const void* psprite, void* pvideomem, u8 width, u8 height, const void* pmasktable)
{
	_transparentColor = ((u8*)pmasktable)[0];
	DrawSprite((void*)psprite, pvideomem, width, height, SPRITE_ALIGNEDTABLE);

	Sleep(1);
}

static void hflipByte(u8 width, u8 height, u8* sprite)
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
static u8 DecodeFlipPixMode0(u8 data)
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
	hflipByte(width, height, data);

	for (int i = 0; i < width*height; i++)
		*data++ = DecodeFlipPixMode0(*data);
}

void cpct_hflipSpriteM0_f(u8 width, u8 height, void* sprite)
{
	cpct_hflipSpriteM0(width, height, (u8*)sprite);
	Sleep(1);
}

static u8 DecodeFlipPixMode1(u8 data)
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
	u8* data = (u8*)sprite;
	hflipByte(width, height, data);

	data = sprite;
	for (int i = 0; i < width*height; i++)
		*data++ = DecodeFlipPixMode1(*data);

	Sleep(1);
}

void cpct_hflipSpriteM1_f(u8 width, u8 height, void* sprite)
{
	cpct_hflipSpriteM1(width, height, (u8*)sprite);
}

static u8 DecodeFlipPixMode2(u8 data)
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
	u8* data = (u8*)sprite;
	hflipByte(width, height, data);

	data = sprite;
	for (int i = 0; i < width*height; i++)
		*data++ = DecodeFlipPixMode2(*data);

	Sleep(1);
}

void cpct_hflipSpriteM2_f(u8 width, u8 height, void* sprite)
{
	cpct_hflipSpriteM2(width, height, (u8*)sprite);
}

static void hflipByteMasked(u8 width, u8 height, u16* sprite)
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
	u8* data = (u8*)sprite;
	hflipByteMasked(width, height, sprite);

	for (int i = 0; i < width*height * 2; i++)
		*data++ = DecodeFlipPixMode0(*data);

	Sleep(1);
}

void cpct_hflipSpriteMaskedM1(u8 width, u8 height, void* sprite)
{
	u8* data = (u8*)sprite;
	hflipByteMasked(width, height, sprite);

	for (int i = 0; i < width*height*2; i++)
		*data++ = DecodeFlipPixMode1(*data);

	Sleep(1);
}

void cpct_hflipSpriteMaskedM2(u8 width, u8 height, void* sprite)
{
	u8* data = (u8*)sprite;
	hflipByteMasked(width, height, sprite);

	for (int i = 0; i < width*height * 2; i++)
		*data++ = DecodeFlipPixMode2(*data);

	Sleep(1);
}

void DrawSprite(void *sprite, void *memory, int cx, int cy, u8 pSpriteMode)
{
	u8* video = (u8*)memory;

	if (pSpriteMode == SPRITE_MASKED)
	{
		WORD* pix = (WORD*)sprite;
		for (int yi = 0; yi < cy; yi++)
		{
			for (int xi = 0; xi < cx; xi++)
			{
				u8 mask = ConvPixCPCtoPC((u8)(*pix));
				u8 sprite = ConvPixCPCtoPC((u8)(*pix >> 8));
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
				*video = ConvPixCPCtoPC(*pix);
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
				u8 pixel = ConvPixCPCtoPC(*pix);
				if (pixel != _transparentColor)
					*video = pixel;

				video++;
				pix++;
			}
			video += (CPC_SCR_CX_BYTES - cx);
		}
	}
}