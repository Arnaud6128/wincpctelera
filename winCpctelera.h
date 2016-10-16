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
#define CPC_INTERRUPT_LINE	(CPC_SCR_CY_LINE/6)

#define CPC_MEM_SIZE		0xFFFF

#define INTERRUPT_MS		33
#define REFRESH_RATE		50

enum
{
	MODE_0,
	MODE_1,
	MODE_2
};

enum
{
	SPRITE_NORMAL,
	SPRITE_MASKED,
	SPRITE_ALIGNEDTABLE
};


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

	u8 _renderVideo[0x4000];
	u8 _renderMode1Video[0x8000];

} SAmstrad;

typedef struct tagSCPCPalette
{
	u8 hw;
	COLORREF rgb;
} SCPCPalette;

extern SCPCPalette _palette[NB_PAL_COLOR];
extern BOOL _curKey;
extern SAmstrad _amstrad;

void CPCTeleraWin();
void MsgLoop();
void Refresh();
void StartInterrupt();

BOOL IsCpcMem(const void* pAddress);
int ConvertPixelPos(int x);
int GetVideoArea(int pScreenAddr);
u8* GetVideoBufferFromAddress(int pScreenAddr);
u8 DecodePixel(u8 pPix);
void CreatePaletteCpc();
void DrawSprite(void *sprite, void *memory, int cx, int cy, u8 pSpriteMode);

