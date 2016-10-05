#include "winCpctelera.h"

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

void cpct_drawCharM1_f(void* video_memory, u8 fg_pen, u8 bg_pen, u8 ascii)
{
	cpct_drawCharM1(video_memory, fg_pen, bg_pen, ascii);
}

void cpct_drawStringM1(void* string, void* video_memory, u8 fg_pen, u8 bg_pen)
{
	DrawString(string, video_memory, fg_pen, bg_pen, 1);
}

void cpct_drawStringM1_f(void* string, void* video_memory, u8 fg_pen, u8 bg_pen)
{
	DrawString(string, video_memory, fg_pen, bg_pen, 1);
}

void cpct_drawCharM2(void* video_memory, u8 pen, u8 ascii)
{
	if (IsCpcMem(video_memory))
		video_memory = GetVideoBufferFromAddress((int)video_memory);

	DisplayFontM2(video_memory, pen, ascii);
}

void cpct_drawStringM2(void* string, void* video_memory, u8 pen)
{
	DrawString(string, video_memory, 0, pen, 2);
}
