#include "winCpctelera.h"

void cpct_fw2hw(void *fw_colour_array, u16 size)
{
	u8* fwPal = (u8*)fw_colour_array;
	for (u16 i = 0; i < size; i++)
		fwPal[i] = cpct_getHWColour(fwPal[i]);
}

void cpct_setPalette(u8* ink_array, u16 ink_array_size)
{
	memcpy(_amstrad._curPal, ink_array, ink_array_size);
	CreatePaletteCpc();
}

void cpct_setPALColour(u8 pen, u8 hw_ink)
{
	_amstrad._curPal[pen] = hw_ink;
	CreatePaletteCpc();
}

u8 cpct_getHWColour(u16 pFW)
{
	return _palette[pFW].hw;
}

void cpct_waitVSYNC()
{
	Sleep(20);
}

u16 cpct_count2VSYNC()
{
	return 0;
}

void cpct_clearScreen(u8 colour_pattern)
{
	memset(GetVideoBufferFromAddress(0xC000), colour_pattern, 0x4000);
}

void cpct_setVideoMode(u8 videoMode)
{
	_amstrad._mode = videoMode;
}

void cpct_setVideoMemoryPage(u8 page_6LSb)
{
	_amstrad._currentPage = page_6LSb;
	Refresh();
}

void cpct_setVideoMemoryOffset(u8 offset)
{
	_amstrad._memOffset = offset;
}

u8* cpct_getScreenPtr(void* screen_start, u8 x, u8 y)
{
	u8* memory = GetVideoBufferFromAddress((int)screen_start);
	return memory + y * CPC_SCR_CX_BYTES + x;
}
