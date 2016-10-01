#include <cpcteleraWin.h>

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