/** Functions to keep code compatibility with CPCTelera and debug */
/** Include to your project */
#include <cpctelera.h>
#include <stdio.h>

#ifdef WINCPCTELERA

#define	WC_STATIC_CONST				const

void AddTraceStr(char* pName, char* pText);
void AddTraceInt(char* pName, int pValue);
void AddTraceHexa(char* pName, int pValue);
void ClearTrace(char* pName);
void DrawRectHelper(int rectId, int x, int y, int cx, int cy, int color);
void CleanRectHelper();

extern u8* wincpct_getMemory(const u8* ptr);
extern void wincpct_wait(int ms);
extern void wincpct_msgLoop();
extern void wincpct_SetLoader(void* loader);
extern void WinCpcteleraLoaderFunction();

#define WinCpcTelera_cleanRect()			CleanRectHelper()
#define WinCpcTelera_drawRect(R,X,Y,W,H,C)	DrawRectHelper(R, X,Y,W,H,C)
#define WinCpcTelera_SetLoader()			wincpct_SetLoader(WinCpcteleraLoaderFunction)
#define WinCpcTelera_GetMemPtr(X)			(u8*)wincpct_getMemory((u8*)X)
#define WinCpcTelera_WaitKey()				{ while(!cpct_isAnyKeyPressed()){ wincpct_wait(10); wincpct_msgLoop(); }; }
#define WinCpcTelera_Process()				wincpct_msgLoop()
#define WinCpcTelera_Wait(X)				wincpct_wait(X)	
#define WinCpcTelera_Stop()					{ while(1){ wincpct_wait(10); wincpct_msgLoop(); }; }
#define WinCpcTelera_PrintStr(N,X)			printf("%s : %s\n", N, X)
#define WinCpcTelera_PrintInt(N,X)			printf("%s : %d\n", N, X)
#define WinCpcTelera_PrintHex(N,X)			printf("%s : 0x%04X\n", N, X)
#define WinCpcTelera_TraceStr(N,X)			AddTraceStr(N, X)
#define WinCpcTelera_TraceInt(N,X)			AddTraceInt(N, X)
#define WinCpcTelera_TraceHex(N,X)			AddTraceHexa(N, X)
#define WinCpcTelera_TraceDel(N)			ClearTrace(N)

#else

#define	WC_STATIC_CONST				static const

#define WinCpcTelera_cleanRect()		
#define WinCpcTelera_drawRect(R,X,Y,W,H,C)
#define WinCpcTelera_SetLoader(X)    
#define WinCpcTelera_GetMemPtr(X)    (u8*)(X)
#define WinCpcTelera_WaitKey()
#define WinCpcTelera_Process()
#define WinCpcTelera_Wait(X)    
#define WinCpcTelera_Stop()
#define WinCpcTelera_PrintStr(N,X)
#define WinCpcTelera_PrintInt(N,X)
#define WinCpcTelera_PrintHex(N,X)
#define WinCpcTelera_TraceStr(N,X)	  
#define WinCpcTelera_TraceInt(N,X)	 
#define WinCpcTelera_TraceHex(N,X)	  
#define WinCpcTelera_TraceDel(N)	 
#endif // WINCPCTELERA