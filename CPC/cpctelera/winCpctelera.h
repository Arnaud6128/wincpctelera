//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
//  Copyright (C) 2016 Arnaud Bouche
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

#define NB_PAL_COLOR		27
#define NB_COLORS			16
#define BORDER_COLOR		16

#define FULL_SCREEN_CX		(BORDER_CX + WIDTH_SCREEN + BORDER_CX)
#define FULL_SCREEN_CY		(BORDER_UP_CY + HEIGHT_SCREEN + BORDER_DW_CY)

#define BORDER_CX			64
#define BORDER_UP_CY		96
#define BORDER_DW_CY		80
#define WIDTH_SCREEN		640
#define HEIGHT_SCREEN		400

#define CPC_SCR_CX_BYTES	80
#define CPC_SCR_CY_LINE		200
#define CPC_INTERRUPT_LINE	(CPC_SCR_CY_LINE/6)

#define CPC_MEM_SIZE		0xFFFF

#define INTERRUPT_MS		(int)(1000/50/6)

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

typedef struct tagSAmstrad
{
	u8 _mode;
	u8 _currentPage;
	u8 _internalTimer;
	TInterrupt _interruptFunction;

	u8 _curPal[NB_COLORS + 1];

	u8 _memOffset;

	u8 _memCPC[CPC_MEM_SIZE];
	u8 _mode1Video[0x8000];

	u8 _renderVideo[0x4000];
	u8 _renderMode1Video[0x8000];

} SAmstrad;

typedef struct tagSCPCPalette
{
	u8 hw;
	COLORREF rgb;
} SCPCPalette;

extern SCPCPalette _palette[NB_PAL_COLOR];
extern BOOL _curKey;
extern SAmstrad _amstrad;

void CPCTeleraWin();
void MsgLoop();
void Refresh();
void StartInterrupt();

BOOL IsCpcMem(const void* pAddress);
int ConvertPixelPos(int x);
int GetVideoArea(int pScreenAddr);
u8* GetVideoBufferFromAddress(int pScreenAddr);
u8 ConvPixCPCtoPC(u8 pPix);
void CreatePaletteCpc();
void DrawSprite(void *sprite, void *memory, int cx, int cy, u8 pSpriteMode);

