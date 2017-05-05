#include <cpctelera.h>

void ExoDecrunch(const u8* source, u8* target) __z88dk_callee 
{
	__asm
		pop af
		pop hl
		pop de

		push af
		push ix

		call _deexo

		pop ix
	__endasm;
}
