#ifndef CPCTELERA_H
#define CPCTELERA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <types.h>

#define __z88dk_fastcall	
#define __z88dk_callee	
#define __naked 

#define CPCT_VMEM_START (u8*)0xC000
#define cpct_pageC0 0x30
#define cpct_page80 0x20
#define cpct_page40 0x10
#define cpct_page00 0x00


void CPCTeleraWin();

/** Memory */
void cpct_memset(void *array, u8  value, u16 size) __z88dk_callee;
void cpct_memset_f8(void *array, u16 value, u16 size) __z88dk_callee;
void cpct_memset_f64(void *array, u16 value, u16 size) __z88dk_callee;
void cpct_memcpy(void *to, const void *from, u16 size) __z88dk_callee;
void cpct_setStackLocation(void* memory) __z88dk_fastcall;

/** Keyboard */
enum cpct_e_keyID;
typedef enum cpct_e_keyID cpct_keyID;

void cpct_scanKeyboard();
void cpct_scanKeyboard_f();
void cpct_scanKeyboard_if();
u8 cpct_isKeyPressed(cpct_keyID key) __z88dk_fastcall;
u8 cpct_isAnyKeyPressed_f();

enum CPCT_HW_Colour {
	HW_BLACK = 0x14, HW_BLUE = 0x04
	, HW_BRIGHT_BLUE = 0x15, HW_RED = 0x1C
	, HW_MAGENTA = 0x18, HW_MAUVE = 0x1D
	, HW_BRIGHT_RED = 0x0C, HW_PURPLE = 0x05
	, HW_BRIGHT_MAGENTA = 0x0D, HW_GREEN = 0x16
	, HW_CYAN = 0x06, HW_SKY_BLUE = 0x17
	, HW_YELLOW = 0x1E, HW_WHITE = 0x00
	, HW_PASTEL_BLUE = 0x1F, HW_ORANGE = 0x0E
	, HW_PINK = 0x07, HW_PASTEL_MAGENTA = 0x0F
	, HW_BRIGHT_GREEN = 0x12, HW_SEA_GREEN = 0x02
	, HW_BRIGHT_CYAN = 0x13, HW_LIME = 0x1A
	, HW_PASTEL_GREEN = 0x19, HW_PASTEL_CYAN = 0x1B
	, HW_BRIGHT_YELLOW = 0x0A, HW_PASTEL_YELLOW = 0x03
	, HW_BRIGHT_WHITE = 0x0B
};

enum cpct_e_keyID
{
	// Matrix Line 00h
	Key_CursorUp = (i16)0x0100,  // Bit 0 (01h) => | 0000 0001 |
	Key_CursorRight = (i16)0x0200,  // Bit 1 (02h) => | 0000 0010 |
	Key_CursorDown = (i16)0x0400,  // Bit 2 (04h) => | 0000 0100 |
	Key_F9 = (i16)0x0800,  // Bit 3 (08h) => | 0000 1000 |
	Key_F6 = (i16)0x1000,  // Bit 4 (10h) => | 0001 0000 |
	Key_F3 = (i16)0x2000,  // Bit 5 (20h) => | 0010 0000 |
	Key_Enter = (i16)0x4000,  // Bit 6 (40h) => | 0100 0000 |
	Key_FDot = (i16)0x8000,  // Bit 7 (80h) => | 1000 0000 |

	// Matrix Line 01h
	Key_CursorLeft = (i16)0x0101,
	Key_Copy = (i16)0x0201,
	Key_F7 = (i16)0x0401,
	Key_F8 = (i16)0x0801,
	Key_F5 = (i16)0x1001,
	Key_F1 = (i16)0x2001,
	Key_F2 = (i16)0x4001,
	Key_F0 = (i16)0x8001,

	// Matrix Line 02h
	Key_Clr = (i16)0x0102,
	Key_OpenBracket = (i16)0x0202,
	Key_Return = (i16)0x0402,
	Key_CloseBracket = (i16)0x0802,
	Key_F4 = (i16)0x1002,
	Key_Shift = (i16)0x2002,
	Key_BackSlash = (i16)0x4002,
	Key_Control = (i16)0x8002,

	// Matrix Line 03h
	Key_Caret = (i16)0x0103,
	Key_Hyphen = (i16)0x0203,
	Key_At = (i16)0x0403,
	Key_P = (i16)0x0803,
	Key_SemiColon = (i16)0x1003,
	Key_Colon = (i16)0x2003,
	Key_Slash = (i16)0x4003,
	Key_Dot = (i16)0x8003,

	// Matrix Line 04h
	Key_0 = (i16)0x0104,
	Key_9 = (i16)0x0204,
	Key_O = (i16)0x0404,
	Key_I = (i16)0x0804,
	Key_L = (i16)0x1004,
	Key_K = (i16)0x2004,
	Key_M = (i16)0x4004,
	Key_Comma = (i16)0x8004,

	// Matrix Line 05h
	Key_8 = (i16)0x0105,
	Key_7 = (i16)0x0205,
	Key_U = (i16)0x0405,
	Key_Y = (i16)0x0805,
	Key_H = (i16)0x1005,
	Key_J = (i16)0x2005,
	Key_N = (i16)0x4005,
	Key_Space = (i16)0x8005,

	// Matrix Line 06h
	Key_6 = (i16)0x0106,
	Joy1_Up = (i16)0x0106,
	Key_5 = (i16)0x0206,
	Joy1_Down = (i16)0x0206,
	Key_R = (i16)0x0406,
	Joy1_Left = (i16)0x0406,
	Key_T = (i16)0x0806,
	Joy1_Right = (i16)0x0806,
	Key_G = (i16)0x1006,
	Joy1_Fire1 = (i16)0x1006,
	Key_F = (i16)0x2006,
	Joy1_Fire2 = (i16)0x2006,
	Key_B = (i16)0x4006,
	Joy1_Fire3 = (i16)0x4006,
	Key_V = (i16)0x8006,

	// Matrix Line 07h
	Key_4 = (i16)0x0107,
	Key_3 = (i16)0x0207,
	Key_E = (i16)0x0407,
	Key_W = (i16)0x0807,
	Key_S = (i16)0x1007,
	Key_D = (i16)0x2007,
	Key_C = (i16)0x4007,
	Key_X = (i16)0x8007,

	// Matrix Line 08h
	Key_1 = (i16)0x0108,
	Key_2 = (i16)0x0208,
	Key_Esc = (i16)0x0408,
	Key_Q = (i16)0x0808,
	Key_Tab = (i16)0x1008,
	Key_A = (i16)0x2008,
	Key_CapsLock = (i16)0x4008,
	Key_Z = (i16)0x8008,

	// Matrix Line 09h
	Joy0_Up = (i16)0x0109,
	Joy0_Down = (i16)0x0209,
	Joy0_Left = (i16)0x0409,
	Joy0_Right = (i16)0x0809,
	Joy0_Fire1 = (i16)0x1009,
	Joy0_Fire2 = (i16)0x2009,
	Joy0_Fire3 = (i16)0x4009,
	Key_Del = (i16)0x8009
};

/** Firmware */
void cpct_reenableFirmware(u16 firmware_ROM_code) __z88dk_fastcall;
u16 cpct_disableFirmware();
void cpct_setInterruptHandler( void(*intHandler)(void) ) __z88dk_fastcall;

/** Sprite */
void cpct_drawSprite(void *sprite, void* memory, u8 width, u8 height) __z88dk_callee;
void cpct_drawSpriteMasked(void *sprite, void* memory, u8 width, u8 height) __z88dk_callee;
void cpct_drawSpriteBlended(void *memory, u8 height, u8 width, void *sprite) __z88dk_callee;
void cpct_drawSolidBox(void *memory, u8 colour_pattern, u8 width, u8 height);
u8 cpct_px2byteM0(u8 px0, u8 px1);

/** Video */
#define cpct_clearScreen(COL) cpct_memset((void*)0xC000, (COL), 0x4000)
#define cpct_setBorder(HWC) cpct_setPALColour (16, (HWC))

void cpct_setVideoMode(u8 videoMode) __z88dk_fastcall;
void cpct_waitVSYNC();
u16 cpct_count2VSYNC();
void cpct_fw2hw(void *fw_colour_array, u16 size) __z88dk_callee;
void cpct_setPalette(u8* ink_array, u16 ink_array_size) __z88dk_callee;
u8 cpct_getHWColour(u16 firmware_colour) __z88dk_fastcall;
void cpct_setPALColour(u8 pen, u8 hw_ink) __z88dk_callee;
void cpct_setVideoMemoryPage(u8 page_codified_in_6LSb) __z88dk_fastcall;
void cpct_setVideoMemoryOffset(u8 offset) __z88dk_fastcall;
u8* cpct_getScreenPtr(void* screen_start, u8 x, u8 y) __z88dk_callee;
void cpct_setVideoMode(u8 videoMode);
void cpct_setVideoMemoryPage(u8 page_6LSb);

/** Random */
u8 cpct_getRandom_mxor_u8();
u16 cpct_getRandom_mxor_u16();
u32 cpct_getRandom_mxor_u32();

#define cpct_rand8  cpct_getRandom_mxor_u8
#define cpct_rand16 cpct_getRandom_mxor_u16
#define cpct_rand32 cpct_getRandom_mxor_u32
#define cpct_rand   cpct_rand8

#define CPCT_ABSOLUTE_LOCATION_AREA(x)	
#define CPCT_RELOCATABLE_AREA()

#endif