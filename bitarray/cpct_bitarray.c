#include <winCpctelera.h>

static u16* GetWord(void *array, int pos, int nbBits)
{
	int offset = pos * nbBits;
	u16* word = ((u16*)array) + offset / 16;
	return word;
}

static u8 GetBitPos(int pos, int nbBits)
{
	int offset = pos * nbBits;
	return offset % 16;
}

static u8 GetBits(void *array, u16 pos, int size)
{
	u8 bitPos = GetBitPos(pos, size);
	u8* byte = (u8*)GetWord(array, pos, size);
	
	//u8 byte = *word >> 

	u8 mask = 0xFFFF >> (16 - size);
	mask = mask << bitPos;
	return (*byte & mask) >> bitPos;
}

void SetBits(void *array, u16 groupPos, u16 value, int size)
{
	u8 bitPos = GetBitPos(groupPos, size);
	u16* word = GetWord(array, groupPos, size);
	
	u8 byte = 0;

	u16 mask = 0xFFFF >> (16 - size);
	mask = ~(mask << bitPos);
	*word = *word & mask;
	*word |= (value << bitPos);
}

u8 cpct_getBit(void *array, u16 pos)
{
	return GetBits(array, pos, 1); 
}

u8 cpct_get2Bits(void *array, u16 pos) 
{
	return GetBits(array, pos, 2);
}

u8 cpct_get4Bits(void *array, u16 pos) 
{
	return GetBits(array, pos, 4);
}

u8 cpct_get6Bits(void *array, u16 pos) 
{
	return GetBits(array, pos, 6);
}

void cpct_setBit(void *array, u16 value, u16 pos)
{
	SetBits(array, pos, value, 1);
}

void cpct_set2Bits(void *array, u16 value, u16 pos) 
{
	SetBits(array, pos, value, 2);
}

void cpct_set4Bits(void *array, u16 value, u16 pos) 
{
	SetBits(array, pos, value, 4);
}

void cpct_set6Bits(void *array, u16 value, u16 pos) 
{
	SetBits(array, pos, value, 6);
}
