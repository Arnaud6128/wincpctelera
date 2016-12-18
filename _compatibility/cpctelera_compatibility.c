#include "cpctelera_compatibility.h"

#ifdef WINCPCTELERA
	extern u8* GetMemory(const void* ptr);
#endif

/*****************************************************/
/*													 */
/*	Get memory pointer					 			 */
/*												     */
/*****************************************************/
u8* WinCpcTelera_GetMemPtr(u8* mem)
{
#ifdef WINCPCTELERA
	return GetMemory(mem);
#else
	return pMem;
#endif // WINCPCTELERA
}
