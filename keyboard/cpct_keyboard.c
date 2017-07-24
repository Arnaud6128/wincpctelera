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

#include <winCpctelera.h>

static void wincpct_scanKeyboard();
static u16 wincpct_getVKey(u16 pCpcKeyID);

BOOL gCurKey;
u8 cpct_keyboardStatusBuffer[10];

typedef struct
{
	u16 cpcKeyID;
	u16 winKeyID;
} SKeyMapping;

static const SKeyMapping sCpctMapKey[] =
{
	{ Key_CursorUp, VK_UP },
	{ Key_CursorRight, VK_RIGHT },
	{ Key_CursorDown, VK_DOWN },
	{ Key_F9, VK_F9 },
	{ Key_F6, VK_F6 },
	{ Key_F3, VK_F3 },
	{ Key_Enter, VK_RETURN },
	{ Key_FDot, VK_OEM_PERIOD },

	{ Key_CursorLeft, VK_LEFT },
	{ Key_Copy, VK_MENU },
	{ Key_F7, VK_F7 },
	{ Key_F8, VK_F8 },
	{ Key_F5, VK_F5 },
	{ Key_F1, VK_F1 },
	{ Key_F2, VK_F2 },
	{ Key_F0, VK_F10 },

	{ Key_Clr,  VK_CONTROL },
	{ Key_OpenBracket, 0 },
	{ Key_Return, VK_RETURN },
	{ Key_CloseBracket, 0 },
	{ Key_F4, VK_F4 },
	{ Key_Shift, VK_SHIFT },
	{ Key_BackSlash, VK_BACK },
	{ Key_Control, VK_CONTROL },

	{ Key_Caret, 0 },
	{ Key_Hyphen, 0 },
	{ Key_At, 0 },
	{ Key_P, 0x50 },
	{ Key_SemiColon, 0 },
	{ Key_Colon, 0 },
	{ Key_Slash, 0 },
	{ Key_Dot, VK_OEM_PERIOD },

	{ Key_0, 0x30 },
	{ Key_9, 0x39 },
	{ Key_O, 0x4F },
	{ Key_I, 0x49 },
	{ Key_L, 0x4C },
	{ Key_K, 0x4B },
	{ Key_M, 0x4D },
	{ Key_Comma, VK_OEM_COMMA },

	{ Key_8, 0x38 },
	{ Key_7, 0x37 },
	{ Key_U, 0x55 },
	{ Key_Y, 0x59 },
	{ Key_H, 0x48 },
	{ Key_J, 0x4A },
	{ Key_N, 0x4E },
	{ Key_Space, VK_SPACE },

	{ Key_6, 0x36 },
	{ Joy1_Up, VK_UP },
	{ Key_5, 0x35 },
	{ Joy1_Down, VK_DOWN },
	{ Key_R, 0x52 },
	{ Joy1_Left, VK_LEFT },
	{ Key_T, 0x54 },
	{ Joy1_Right, VK_RIGHT },
	{ Key_G, 0x47 },
	{ Joy1_Fire1, VK_SPACE },
	{ Key_F, 0x46 },
	{ Joy1_Fire2, VK_CONTROL },
	{ Key_B, 0x42 },
	{ Joy1_Fire3, VK_CONTROL },
	{ Key_V, 0x56 },

	{ Key_4, 0x34 },
	{ Key_3, 0x33 },
	{ Key_E, 0x45 },
	{ Key_W, 0x57 },
	{ Key_S, 0x53 },
	{ Key_D, 0x44 },
	{ Key_C, 0x43 },
	{ Key_X, 0x58 },

	{ Key_1, 0x31 },
	{ Key_2, 0x32 },
	{ Key_Esc, VK_ESCAPE },
	{ Key_Q, 0x51 },
	{ Key_Tab, VK_TAB },
	{ Key_A, 0x41 },
	{ Key_CapsLock, VK_CAPITAL },
	{ Key_Z, 0x5A },

	{ Joy0_Up, VK_UP },
	{ Joy0_Down, VK_DOWN },
	{ Joy0_Left, VK_LEFT },
	{ Joy0_Right, VK_RIGHT },
	{ Joy0_Fire1, VK_SPACE },
	{ Joy0_Fire2, VK_CONTROL },
	{ Joy0_Fire3, VK_CONTROL },
	{ Key_Del, VK_DELETE },
};

void cpct_scanKeyboard_f()
{
	wincpct_scanKeyboard();
}

void cpct_scanKeyboard_i()
{
	wincpct_scanKeyboard();
}

void cpct_scanKeyboard_if()
{
	wincpct_scanKeyboard();
}

void cpct_scanKeyboard()
{
	wincpct_scanKeyboard();
}

u8 cpct_isKeyPressed(cpct_keyID key)
{
	wincpct_msgLoop();
	
	u16 vKey = wincpct_getVKey(key);
	BOOL isKeyPressed = (GetAsyncKeyState(vKey) != 0);

	if (isKeyPressed == FALSE)
		isKeyPressed = (wincpct_getAsyncJoyState(vKey) != 0);

	gCurKey = FALSE;
	return isKeyPressed;
}

u8 cpct_isAnyKeyPressed()
{
	wincpct_msgLoop();

	BOOL isKeyPressed = (gCurKey != FALSE);
	gCurKey = FALSE;
	return isKeyPressed;
}

u8 cpct_isAnyKeyPressed_f()
{
	return cpct_isAnyKeyPressed();
}

static u16 wincpct_getVKey(u16 pCpcKeyID)
{
	for (int i = 0; i < sizeof(sCpctMapKey) / sizeof(SKeyMapping); i++)
	{
		if (sCpctMapKey[i].cpcKeyID == pCpcKeyID)
		{
			return sCpctMapKey[i].winKeyID;
		}
	}
	return 0;
}

static void wincpct_scanKeyboard()
{
	memset(cpct_keyboardStatusBuffer, 0xFF, sizeof(cpct_keyboardStatusBuffer));
	wincpct_msgLoop();
}

u16 wincpct_getCpcKey(u16 pVKeyID)
{
	for (int i = 0; i < sizeof(sCpctMapKey) / sizeof(SKeyMapping); i++)
	{
		if (sCpctMapKey[i].winKeyID == pVKeyID)
		{
			return sCpctMapKey[i].cpcKeyID;
		}
	}
	return 0;
}