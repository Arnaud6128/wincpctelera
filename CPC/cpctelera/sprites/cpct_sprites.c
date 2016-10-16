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
			u8 pixSrc = DecodePixel(*src);

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

u8 cpct_px2byteM0(u8 px0, u8 px1)
{
	return (px1 << 4) | px0;
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
}

void cpct_drawSpriteMasked(void *sprite, void* memory, u8 width, u8 height)
{
	DrawSprite(sprite, memory, width, height, SPRITE_MASKED);
}

void cpct_drawSolidBox(void *memory, u8 colour_pattern, u8 width, u8 height)
{
	if (IsCpcMem(memory))
		memory = GetVideoBufferFromAddress((int)memory);

	u8* video = (u8*)memory;

	for (int yi = 0; yi < height; yi++)
	{
		for (int xi = 0; xi < width; xi++)
		{
			*video = colour_pattern;
			video++;
		}
		video += (CPC_SCR_CX_BYTES - width);
	}
}

void cpct_drawSpriteMaskedAlignedTable(const void* psprite, void* pvideomem, u8 width, u8 height, const void* pmasktable)
{
	_transparentColor = ((u8*)pmasktable)[0];
	DrawSprite((void*)psprite, pvideomem, width, height, SPRITE_ALIGNEDTABLE);
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

static u8 convertPixMode0(u8 data)
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
		*data++ = convertPixMode0(*data);
}

void cpct_hflipSpriteM0_f(u8 width, u8 height, void* sprite)
{
	cpct_hflipSpriteM0(width, height, (u8*)sprite);
}

static u8 convertPixMode1(u8 data)
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
	{
		*data++ = convertPixMode1(*data);
	}
}

void cpct_hflipSpriteM1_f(u8 width, u8 height, void* sprite)
{
	cpct_hflipSpriteM1(width, height, (u8*)sprite);
}

static u8 convertPixMode2(u8 data)
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
	{
		*data++ = convertPixMode2(*data);
	}
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
		*data++ = convertPixMode0(*data);
}

void cpct_hflipSpriteMaskedM1(u8 width, u8 height, void* sprite)
{
	u8* data = (u8*)sprite;
	hflipByteMasked(width, height, sprite);

	for (int i = 0; i < width*height*2; i++)
		*data++ = convertPixMode1(*data);
}

void cpct_hflipSpriteMaskedM2(u8 width, u8 height, void* sprite)
{
	u8* data = (u8*)sprite;
	hflipByteMasked(width, height, sprite);

	for (int i = 0; i < width*height * 2; i++)
		*data++ = convertPixMode2(*data);
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
				u8 mask = DecodePixel((u8)(*pix));
				u8 sprite = DecodePixel((u8)(*pix >> 8));
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
				*video = DecodePixel(*pix);
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
				u8 pixel = DecodePixel(*pix);
				if (pixel != _transparentColor)
					*video = pixel;

				video++;
				pix++;
			}
			video += (CPC_SCR_CX_BYTES - cx);
		}
	}
}