#include <winCpctelera.h>

BOOL IsCpcMem(const void* pAddress)
{
	return ((int)pAddress < CPC_MEM_SIZE);
}

static u8* GetMemory(const void* ptr)
{
	if (IsCpcMem(ptr))
		return (u8*)(_amstrad._memCPC) + (int)ptr;

	return (u8*)ptr;
}

void cpct_memcpy(void* to, const void* from, u16 size)
{
	to = GetMemory(to);
	from = GetMemory(from);

	memcpy_s(to, size, from, size);
}

void cpct_memset_f64(void *array, u16 value, u16 size)
{
	u16* data = (u16*)GetMemory(array);

	for (int i = 0; i < size; i++)
		data[i] = value;
}

void cpct_memset_f8(void *array, u16 value, u16 size)
{
	cpct_memset(array, (u8)value, size);
}

void cpct_memset(void *array, u8 value, u16 size)
{
	u8* data = (u8*)GetMemory(array);
	memset(data, value, size);
}

void cpct_setStackLocation(void* memory)
{
	CPCTeleraWin();
}