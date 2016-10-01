#include "cpcteleraWin.h"

u8 cpct_akp_songLoopTimes;
u8 cpct_akp_digidrumStatus;
u8 cpct_keyboardStatusBuffer[10];

void cpct_memcpy(void* to, const void* from, u16 size)
{
	if (IsCpcMem(to))
		to = (u8*)(_amstrad._memVideo) + (int)to;
		
	if (IsCpcMem(from))
		from = (u8*)(_amstrad._memVideo) + (int)from;
		
	memcpy_s(to, size, from, size);
}

void cpct_memset_f64(void *array, u16 value, u16 size)
{
	cpct_memset(array, value, size);
}

void cpct_memset_f8(void *array, u16 value, u16 size)
{
	cpct_memset(array, value, size);
}

void cpct_memset(void *array, u8 value, u16 size)
{
	u8* data = (u8*)array;
	if (IsCpcMem(array))
		data = (u8*)(_amstrad._memVideo) + (int)array;

	memset(data, value, size);
}

void cpct_setStackLocation(void* memory)
{
	CPCTeleraWin();
}

u8 cpct_getRandom_mxor_u8()
{
	return (u8)rand();
}

u16 cpct_getRandom_mxor_u16()
{
	return (u16)rand();
}

u32 cpct_getRandom_mxor_u32()
{
	return (u32)rand();
}

void cpct_setInterruptHandler(void(*intHandler)(void))
{
	_amstrad._interruptFunction = intHandler;
}

void cpct_reenableFirmware(u16 firmware_ROM_code)
{
	MsgLoop();
}

u16 cpct_disableFirmware()
{
	srand(GetTickCount());
	CPCTeleraWin();
	return 0;
}

void cpct_scanKeyboard_f()
{
	ScanKeyboard();
}

void cpct_scanKeyboard_if()
{
	ScanKeyboard();
}

void cpct_scanKeyboard()
{
	ScanKeyboard();
}

u8 cpct_isKeyPressed(cpct_keyID key)
{
	MsgLoop();
	return GetAsyncKeyState(GetVKey(key)) != 0 ? TRUE : FALSE;
}

u8 cpct_isAnyKeyPressed()
{
	MsgLoop();
	BOOL isKeyPressed = (_curKey != FALSE);
	return isKeyPressed;
}

u8 cpct_isAnyKeyPressed_f()
{
	return cpct_isAnyKeyPressed();
}

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
	FillBorder(NULL);
}

void cpct_setPALColour(u8 pen, u8 hw_ink)
{
	_amstrad._curPal[pen] = hw_ink;
	CreatePaletteCpc();
	FillBorder(NULL);
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

u8 cpct_getHWColour(u16 pFW)
{
	return _palette[pFW].hw;
}

u8 cpct_px2byteM0(u8 px0, u8 px1)
{
	UCHAR bit0 = (px0 & 0x01);
	UCHAR bit1 = (px0 & 0x02) >> 1;
	UCHAR bit2 = (px0 & 0x04) >> 2;
	UCHAR bit3 = (px0 & 0x08) >> 3;
	UCHAR bita = (px1 & 0x01);
	UCHAR bitb = (px1 & 0x02) >> 1;
	UCHAR bitc = (px1 & 0x04) >> 2;
	UCHAR bitd = (px1 & 0x08) >> 3;

	return bit0 << 7 | bita << 6 | bit2 << 5 | bitc << 4 | bit1 << 3 | bitb << 2 | bit3 << 1 | bitd;
}

void cpct_drawCharM0(void* video_memory, u8 fg_pen, u8 bg_pen, u8 ascii)
{
	if (IsCpcMem(video_memory))
		video_memory = GetVideoBufferFromAddress((int)video_memory);

	DisplayFontM0(video_memory, fg_pen, bg_pen, ascii);
}

void cpct_drawStringM0(void* string, void* video_memory, u8 fg_pen, u8 bg_pen)
{
	DrawString(string, video_memory, fg_pen, bg_pen, 0);
}

void cpct_drawCharM1(void* video_memory, u8 fg_pen, u8 bg_pen, u8 ascii)
{
	if (IsCpcMem(video_memory))
		video_memory = GetVideoBufferFromAddress((int)video_memory);

	DisplayFontM1(video_memory, fg_pen, bg_pen, ascii);
}

void cpct_drawStringM1(void* string, void* video_memory, u8 fg_pen, u8 bg_pen)
{
	DrawString(string, video_memory, fg_pen, bg_pen, 1);
}

void cpct_drawCharM2(void* video_memory, u8 fg_pen, u8 bg_pen, u8 ascii)
{
	if (IsCpcMem(video_memory))
		video_memory = GetVideoBufferFromAddress((int)video_memory);

	DisplayFontM2(video_memory, fg_pen, bg_pen, ascii);
}

void cpct_drawStringM2(void* string, void* video_memory, u8 fg_pen, u8 bg_pen)
{
	DrawString(string, video_memory, fg_pen, bg_pen, 2);
}

u8* cpct_getScreenPtr(void* screen_start, u8 x, u8 y)
{
	u8* memory = GetVideoBufferFromAddress((WORD)screen_start);
	return memory + y * CPC_SCR_CX_BYTES + x;
}

void cpct_waitVSYNC()
{
	Sleep(20);
}

u16 cpct_count2VSYNC()
{
	return 0;
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

void cpct_clearScreen(u8 colour_pattern)
{
	memset(GetVideoBufferFromAddress(0xC000), M0byte2px(colour_pattern), 0x4000);
}

void cpct_drawSolidBox(void *memory, u8 colour_pattern, u8 width, u8 height)
{
	if (IsCpcMem(memory))
		memory = GetVideoBufferFromAddress((int)memory);

	UCHAR* video = (UCHAR*)memory;
	colour_pattern = M0byte2px(colour_pattern);

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

void cpct_setVideoMode(u8 videoMode)
{
	_amstrad._mode = videoMode;
}

void cpct_setVideoMemoryPage(u8 page_6LSb)
{
	_amstrad._currentPage = page_6LSb;
	InvalidateRect(_hWnd, NULL, FALSE);
}

void cpct_akp_musicInit(void* songdata)
{
}

void cpct_akp_musicPlay() 
{
}

void cpct_akp_stop()
{
}

void cpct_akp_SFXInit(void* sfx_song_data) 
{
}

void cpct_akp_SFXStopAll()
{
}

void cpct_akp_SFXStop(u8 stop_bitmask)
{
}

void cpct_akp_SFXPlay(u8 sfx_num, u8 volume, u8 note, u8 speed, u16 inverted_pitch, u8 channel_bitmask)
{
}

u16 cpct_akp_SFXGetInstrument(u8 channel_bitmask)
{
	return 0;
}

void cpct_akp_setFadeVolume(u8 volume) 
{
}
