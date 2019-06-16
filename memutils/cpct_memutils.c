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

BOOL wincpct_isCpcMem(const void* pAddress)
{
	return ((int)pAddress < CPC_MEM_SIZE);
}

BOOL wincpct_isVideoMem(const void* pAddress)
{
	int videoAdress = wincpct_getPageAddress(gAmstrad._currentPage);
	return ((int)pAddress >= videoAdress && (int)pAddress <= (videoAdress + CPC_BANK_SIZE));
}

u8* wincpct_getMemory(const void* ptr)
{
	if (wincpct_isCpcMem(ptr))
		return (u8*)(gAmstrad._memCPC) + (int)ptr;

	return (u8*)ptr;
}

BOOL wincpct_isInternaVideoMem(const void* pAddress)
{
	return (int)pAddress >= (int)(gAmstrad._memCPC + 0xC000);
}

void cpct_memcpy(void* to, const void* from, u16 size)
{
	BOOL isToVideo = wincpct_isInternaVideoMem(to);
	BOOL isFromVideo = wincpct_isInternaVideoMem(from);

	to = wincpct_getMemory(to);
	from = wincpct_getMemory(from);

	if (isToVideo == isFromVideo)
		memcpy_s(to, size, from, size);
	else
	{
		u8* fromPix = from;
		u8* toPix = to;

		for (u16 i = 0; i < size; i++)
		{
			u8 conv = wincpct_convPixSpritePCtoCPC(fromPix[i]);
			toPix[i] = conv;
		}
	}
}

void cpct_memset_f64(void *array, u16 value, u16 size)
{
	u16* data = (u16*)wincpct_getMemory(array);

	for (int i = 0; i < size/2; i++)
		data[i] = value;
}

void cpct_memset_f8(void *array, u16 value, u16 size)
{
	cpct_memset(array, (u8)value, size);
}

void cpct_memset(void *array, u8 value, u16 size)
{	
	if (wincpct_isVideoMem(array))
	{
		int address = (int)array;
		for (int i = 0; i < size; i++) 
		{
			u8* videoData = wincpct_getVideoBufferFromAddress(address + i);
			*videoData = value;
		}
	}
	else
	{
		u8* data = (u8*)wincpct_getMemory(array);
		memset(data, value, size);
	}
}

void cpct_setStackLocation(void* memory)
{
	wincpct_CPCTeleraWin();
}

static void switchBank(u8 indexMemCpc, u8 bankCur, u8 bankNext)
{
	u8* addressMemCpc = gAmstrad._memCPC + indexMemCpc*CPC_BANK_SIZE;

	/** Save current memory to bank */
	memcpy_s(&gAmstrad._bankCPC[bankCur], CPC_BANK_SIZE, addressMemCpc, CPC_BANK_SIZE);
	

	/** Copy data from bank to memory */
	memcpy_s(addressMemCpc, CPC_BANK_SIZE, &gAmstrad._bankCPC[bankNext], CPC_BANK_SIZE);
}

void cpct_pageMemory(u8 configAndBankValue)
{
	wincpct_CPCTeleraWin();

	static u8 sRamCfg[8][4] = {
		// RAMCFG_0: 0000-3FFF -> RAM_0, 4000-7FFF -> RAM_1, 8000-BFFF -> RAM_2, C000-FFFF -> RAM_3
		{ 0, 1, 2, 3 },
		// RAMCFG_1: 0000-3FFF -> RAM_0, 4000-7FFF -> RAM_1, 8000-BFFF -> RAM_2, C000-FFFF -> RAM_7
		{ 0, 1, 2, 7 },
		// RAMCFG_2: 0000-3FFF -> RAM_4, 4000-7FFF -> RAM_5, 8000-BFFF -> RAM_6, C000-FFFF -> RAM_7
		{ 4, 5, 6, 7 },
		// RAMCFG_3: 0000-3FFF -> RAM_0, 4000-7FFF -> RAM_3, 8000-BFFF -> RAM_2, C000-FFFF -> RAM_7
		{ 0, 3, 2, 7 },
		// RAMCFG_4: 0000-3FFF -> RAM_0, 4000-7FFF -> RAM_4, 8000-BFFF -> RAM_2, C000-FFFF -> RAM_3
		{ 0, 4, 2, 3 },
		// RAMCFG_5: 0000-3FFF -> RAM_0, 4000-7FFF -> RAM_5, 8000-BFFF -> RAM_2, C000-FFFF -> RAM_3
		{ 0, 5, 2, 3 },
		// RAMCFG_6: 0000-3FFF -> RAM_0, 4000-7FFF -> RAM_6, 8000-BFFF -> RAM_2, C000-FFFF -> RAM_3
		{ 0, 6, 2, 3 },
		// RAMCFG_7: 0000-3FFF -> RAM_0, 4000-7FFF -> RAM_7, 8000-BFFF -> RAM_2, C000-FFFF -> RAM_3
		{ 0, 7, 2, 3 }
	};

	configAndBankValue &= 0x07;
	if (gAmstrad._currentBank != configAndBankValue)
	{
		u8* curBankConfig = sRamCfg[gAmstrad._currentBank];
		u8* nextBankConfig = sRamCfg[configAndBankValue];

		for (int i = 0; i < 4; i++)
		{
			if (curBankConfig[i] != nextBankConfig[i])
				switchBank(i, curBankConfig[i], nextBankConfig[i]);
		}

		gAmstrad._currentBank = configAndBankValue;
	}

}