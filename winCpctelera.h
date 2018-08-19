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

#include <cpctelera.h>

#define CPCTELERA_VER	1.4.2

#define	_USEWINGDI		1
//#define _USESDL		1

#ifdef _USESDL
	#include <SDL.h>
#endif

#include <windows.h>

#define NB_COLORS			16
#define BORDER_COLOR		16

#define COEF				2
#define BORDER_CX			(32*COEF)
#define BORDER_UP_CY		(41*COEF)
#define BORDER_DW_CY		(29*COEF)
#define WIDTH_SCREEN		(320*COEF)
#define HEIGHT_SCREEN		(200*COEF)

#define FULL_SCREEN_CX		(BORDER_CX + WIDTH_SCREEN + BORDER_CX) // 384 * 2
#define FULL_SCREEN_CY		(BORDER_UP_CY + HEIGHT_SCREEN + BORDER_DW_CY) // 270*2

#define CPC_NB_BANKS		8
#define CPC_BANK_SIZE		0x4000
#define CPC_MEM_SIZE		0x10000

#define INTERRUPT_PER_VBL	6									// 6 interruptions per refresh screen
#define	REFRESH_MS			(1000/50)							// 50 hz
#define INTERRUPT_MS		(REFRESH_MS/INTERRUPT_PER_VBL)		// 3ms
		
#define CPC_SCR_CX_BYTES	80
#define CPC_SCR_CY_LINE		200
#define CPC_INTERRUPT_LINE	(CPC_SCR_CY_LINE/INTERRUPT_PER_VBL)

#define wincpct_getCurrentVideoMode() gAmstrad._curVideoConf._videoMode

enum
{
	MODE_0,
	MODE_1,
	MODE_2
};

enum
{
	SPRITE_NORMAL,
	SPRITE_MASKED,
	SPRITE_ALIGNEDTABLE
};

typedef void(*TInterrupt)(void);

typedef struct tagVideoConf
{
	u8 _videoMode;
	u8 gCpcPalette[NB_COLORS + 1];
} SVideoConf;

typedef struct tagSAmstrad
{
	u8 _currentBank;
	u8 _currentPage;
	u8 _internalTimer;
	TInterrupt _interruptFunction;

	SVideoConf _curVideoConf;

	u8 _memOffset;
	u8 _memCPC[CPC_MEM_SIZE];

	u8 _bankCPC[CPC_NB_BANKS][CPC_BANK_SIZE];
} SAmstrad;

typedef struct tagSCPCPalette
{
	u8 fw;
	u8 hw;
	COLORREF rgb;
} SCPCPalette;

extern SCPCPalette gCpcPalette[];
extern BOOL gCurKey;
extern SAmstrad gAmstrad;

/* wincpctelera */
void wincpct_CPCTeleraWin();
void wincpct_msgLoop();
void wincpct_setInterruptFunction(void(*intHandler)(void));
void wincpct_startInterrupt();
u8 wincpct_getAsyncJoyState(u16 vKey);
void wincpct_wait(int ms);
void wincpct_waitVSync();
void wincpct_createPaletteCpc();

/* cpct_sprite */
u8 wincpct_convPixSpriteCPCtoPC(u8 pix);
u8 wincpct_convPixSpritePCtoCPC(u8 pix);
void wincpct_drawSprite(void *sprite, void *memory, int cx, int cy, u8 pSpriteMode);

/* cpct_memutils */
BOOL wincpct_isCpcMem(const void* pAddress);
u8* wincpct_getMemory(const void* ptr);

/* cpct_video */
u8* wincpct_getVideoBufferFromAddress(int pScreenAddr);
void wincpct_applyLSBOffset(u8* buffVideo);
int wincpct_getVideoArea(int pScreenAddr);
int wincpct_getPageAddress(int pPage);