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

#include <windows.h>
#include <cpctelera.h>

#define	_USEWINGDI			1
//#define _USESDL				1

#ifdef _USESDL
	#include <sdl.h>
#endif

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

#define CPC_BANK_SIZE		0x4000
#define CPC_MEM_SIZE		0x10000

#define INTERRUPT_PER_VBL	6									// 6 interruptions per refresh screen
#define	REFRESH_MS			(1000/50)							// 50 hz
#define INTERRUPT_MS		(REFRESH_MS/INTERRUPT_PER_VBL)		// 3ms
		
#define CPC_SCR_CX_BYTES	80
#define CPC_SCR_CY_LINE		200
#define CPC_INTERRUPT_LINE	(CPC_SCR_CY_LINE/INTERRUPT_PER_VBL)

#define GetCurrentVideoMode() _amstrad._curVideoConf._videoMode

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
	u8 _palette[NB_COLORS + 1];
} SVideoConf;

typedef struct tagSAmstrad
{
	u8 _currentPage;
	u8 _internalTimer;
	TInterrupt _interruptFunction;

	SVideoConf _curVideoConf;

	u8 _memOffset;
	u8 _memCPC[CPC_MEM_SIZE];
} SAmstrad;

typedef struct tagSCPCPalette
{
	u8 fw;
	u8 hw;
	COLORREF rgb;
} SCPCPalette;

extern SCPCPalette _palette[];
extern BOOL _curKey;
extern SAmstrad _amstrad;

void CPCTeleraWin();
void MsgLoop();
void SetInterruptFunction(void(*intHandler)(void));
void StartInterrupt();
