#include <winCpctelera.h>

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

	DrawSprite(sprite, memory, width, height, FALSE);
}

void cpct_drawSpriteMasked(void *sprite, void* memory, u8 width, u8 height)
{
	DrawSprite(sprite, memory, width, height, TRUE);
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
	// TODO
}

void cpct_hflipSprite(u8 width, u8 height, u8* sprite)
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

void cpct_hflipSpriteM0(u8 width, u8 height, void* sprite)
{
	cpct_hflipSprite(width, height, (u8*)sprite);
}

void cpct_hflipSpriteM0_f(u8 width, u8 height, void* sprite)
{
	cpct_hflipSprite(width, height, (u8*)sprite);
}

void cpct_hflipSpriteM1(u8 width, u8 height, void* sprite)
{
	cpct_hflipSprite(width, height, (u8*)sprite);
}

void cpct_hflipSpriteM1_f(u8 width, u8 height, void* sprite)
{
	cpct_hflipSprite(width, height, (u8*)sprite);
}

void cpct_hflipSpriteM2(u8 width, u8 height, void* sprite)
{
	cpct_hflipSprite(width, height, (u8*)sprite);
}

void cpct_hflipSpriteM2_f(u8 width, u8 height, void* sprite)
{
	cpct_hflipSprite(width, height, (u8*)sprite);
}

void cpct_hflipSpriteMasked(u8 width, u8 height, u16* sprite)
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
	cpct_hflipSpriteMasked(width, height, (u16*)sprite);
}

void cpct_hflipSpriteMaskedM1(u8 width, u8 height, void* sprite)
{
	cpct_hflipSpriteMasked(width, height, (u16*)sprite);
}

void cpct_hflipSpriteMaskedM2(u8 width, u8 height, void* sprite)
{
	cpct_hflipSpriteMasked(width, height, (u16*)sprite);
}