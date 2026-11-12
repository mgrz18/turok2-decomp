#include "common.h"
#include "structs/game.h"
#include "globals/engine.h"

typedef struct {
    u8 r, g, b, a;
} Color32;

#ifdef TARGET_PORT
typedef struct Screen {
/* the view at the host offsets of CCamera (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Screen(m) __builtin_offsetof(CCamera, m)
    union {
        struct { u8 _hp_m_Width[T2_HOFF_Screen(m_Viewport.m_Width)]; f32 m_Width; }; /* N64 0x288 */
        struct { u8 _hp_m_Height[T2_HOFF_Screen(m_Viewport.m_Height)]; f32 m_Height; }; /* N64 0x28C */
        struct { u8 _hp_m_X[T2_HOFF_Screen(m_Viewport.m_ScreenPosX)]; f32 m_X; }; /* N64 0x290 */
        struct { u8 _hp_m_Y[T2_HOFF_Screen(m_Viewport.m_ScreenPosY)]; f32 m_Y; }; /* N64 0x294 */
        struct { u8 _hp_m_FlashAlpha[T2_HOFF_Screen(m_Flash)]; u8 m_FlashAlpha; }; /* N64 0x530 */
        struct { u8 _hp_m_FlashFade[T2_HOFF_Screen(m_FlashDec)]; u8 m_FlashFade; }; /* N64 0x531 */
    };
} Screen;
#else
typedef struct {
    /* 0x000 */ u8 _pad0[0x288];
    /* 0x288 */ f32 m_Width;
    /* 0x28C */ f32 m_Height;
    /* 0x290 */ f32 m_X;
    /* 0x294 */ f32 m_Y;
    /* 0x298 */ u8 _pad298[0x530 - 0x298];
    /* 0x530 */ u8 m_FlashAlpha;
    /* 0x531 */ u8 m_FlashFade;
} Screen;
#endif

void CEngineApp__DrawFilledRect(void *, Color32 *, s32, s32, s32, s32);

/* Fades out a red flash over the screen area and draws it while it lasts. */
void CCamera__DrawHitFlash(Screen *scr) {
    Color32 color;

    if (scr->m_FlashAlpha > scr->m_FlashFade) {
        scr->m_FlashAlpha -= (u32)(scr->m_FlashFade * gFrameIncrement);
    } else {
        scr->m_FlashAlpha = 0;
    }
    if (scr->m_FlashAlpha != 0) {
        color.r = 0xFF;
        color.g = 0;
        color.b = 0;
        color.a = scr->m_FlashAlpha;
        CEngineApp__DrawFilledRect(&gEngineApp, &color, scr->m_X, scr->m_Y,
                      scr->m_X + scr->m_Width, scr->m_Y + scr->m_Height);
    }
}
