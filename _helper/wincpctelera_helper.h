/** Functions to keep code compatibility with CPCTelera and debug */
/** Include to your project */

#include <cpctelera.h>
#include <stdio.h>

u8* WinCpcTelera_GetMemPtr(u16 memAddress);

#ifdef WINCPCTELERA
	extern void Wait(int ms);
	extern void MsgLoop();

	#define WinCpcTelera_Process()		MsgLoop()
	#define WinCpcTelera_Wait(X)		Wait(X)
	#define WinCpcTelera_Stop()		{ while(1){ Wait(10); MsgLoop(); }; }
	#define WinCpcTelera_PrintStr(N,X)	printf("%s : %s\n", N, X)
	#define WinCpcTelera_PrintInt(N,X)	printf("%s : %d\n", N, X)
	#define WinCpcTelera_PrintHex(N,X)	printf("%s : 0x%04X\n", N, X)
#else
	#define WinCpcTelera_Process()
	#define WinCpcTelera_Wait(X)	
	#define WinCpcTelera_Stop()
	#define WinCpcTelera_PrintStr(X)
	#define WinCpcTelera_PrintInt(X)
	#define WinCpcTelera_PrintHex(X)
#endif // WINCPCTELERA
