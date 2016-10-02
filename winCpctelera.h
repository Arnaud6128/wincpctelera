#include <windows.h>
#include <cpctelera.h>

#define	_USEWINGDI			1
//#define _USESDL				1

#ifdef _USESDL
	#include <sdl.h>
#endif

#define NB_PAL_COLOR		27
#define NB_COLORS			16
#define BORDER_COLOR		16

#define FULL_SCREEN_CX		(BORDER_CX + WIDTH_SCREEN + BORDER_CX)
#define FULL_SCREEN_CY		(BORDER_UP_CY + HEIGHT_SCREEN + BORDER_DW_CY)

#define BORDER_CX			64
#define BORDER_UP_CY		96
#define BORDER_DW_CY		80
#define WIDTH_SCREEN		640
#define HEIGHT_SCREEN		400

#define CPC_SCR_CX_BYTES	80
#define CPC_SCR_CY_LINE		200

#define CPC_MEM_SIZE		0xFFFF

#define MODE_0				0
#define MODE_1				1
#define MODE_2				2

#define INTERRUPT_MS		33

typedef void(*TInterrupt)(void);

typedef struct tagSAmstrad
{
	u8 _mode;
	u8 _currentPage;
	u8 _internalTimer;
	TInterrupt _interruptFunction;

	u8 _curPal[NB_COLORS + 1];

	u8 _memOffset;

	u8 _memCPC[CPC_MEM_SIZE];
	u8 _mode1Video[0x8000];

} SAmstrad;

typedef struct tagSCPCPalette
{
	u8 hw;
	COLORREF rgb;
} SCPCPalette;

extern const SCPCPalette _palette[NB_PAL_COLOR];
extern BOOL _curKey;
extern SAmstrad _amstrad;
extern u8 cpct_keyboardStatusBuffer[10];

void CPCTeleraWin();
void MsgLoop();
void Refresh();
void StartInterrupt();
BOOL IsCpcMem(const void* pAddress);
int ConvertPixelPos(int x);

void CreatePaletteCpc();
void DrawString(void* string, void* video_memory, u8 fg_pen, u8 bg_pen, int pMode);
void DrawSprite(void *sprite, void *memory, int cx, int cy, BOOL pMasked);

void DisplayFontM0(u8* pVideo, u8 fgPen, u8 bgPen, char pChara);
void DisplayFontM1(u8* pVideo, u8 fgPen, u8 bgPen, char pChara);
void DisplayFontM2(u8* pVideo, u8 pPen, char pChara);
void ScanKeyboard();
u16 GetVKey(u16 pCpcKeyID);

u8* GetVideoBufferFromAddress(int pScreenAddr);
