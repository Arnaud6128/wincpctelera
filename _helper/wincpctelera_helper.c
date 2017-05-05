#include "wincpctelera_helper.h"

#ifdef WINCPCTELERA
	extern u8* wincpct_getMemory(const void* ptr);
#endif

/*****************************************************/
/*													 */
/*	Get memory pointer					 			 */
/*												     */
/*****************************************************/
u8* WinCpcTelera_GetMemPtr(u16 memAddress)
{
#ifdef WINCPCTELERA
	return wincpct_getMemory((u8*)memAddress);
#else
	return (u8*)memAddress;
#endif // WINCPCTELERA
}
