#include <cpctelera.h>

void cpct_memcpy(void* to, const void* from, u16 size)
{
}

void cpct_memset(void *array, u8  value, u16 size)
{
}

u8 cpct_rand8()
{
	return 0;
}

void cpct_setInterruptHandler(void(*intHandler)(void))
{

}

void cpct_reenableFirmware(u16 firmware_ROM_code)
{

}

u16 cpct_disableFirmware()
{
	return 0;
}

void cpct_scanKeyboard_f()
{

}

void cpct_scanKeyboard()
{

}

u8 cpct_isKeyPressed(cpct_keyID key)
{
	return 0;
}

u8 cpct_isAnyKeyPressed_f(cpct_keyID key)
{
	return 0;
}

void cpct_setPalette(u8* ink_array, u16 ink_array_size)
{

}

void cpct_setPALColour(u8 pen, u8 hw_ink)
{

}

u8* cpct_getScreenPtr(void* screen_start, u8 x, u8 y)
{
	return NULL;
}

void cpct_waitVSYNC()
{

}

void cpct_drawSprite(void *sprite, void* memory, u8 width, u8 height)
{

}

void cpct_drawSpriteMasked(void *sprite, void* memory, u8 width, u8 height)
{

}

void cpc_UnExo(const u8 *source, u8* dest)
{

}

void cpct_drawSolidBox(void *memory, u8 colour_pattern, u8 width, u8 height)
{

}