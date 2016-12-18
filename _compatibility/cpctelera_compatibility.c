#include "cpctelera_compatibility.h"

#ifdef WINCPCTELERA
	extern u8* GetMemory(const void* ptr);
#endif

/*****************************************************/
/*													 */
/*	Get memory pointer					 			 */
/*												     */
/*****************************************************/
u8* WinCpcTelera_GetMemPtr(u16 memAddress)
{
#ifdef WINCPCTELERA
	return GetMemory((u8*)memAddress);
#else
	return pMem;
#endif // WINCPCTELERA
}
