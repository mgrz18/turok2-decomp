#include "common.h"
#include "globals/engine.h"

typedef struct {
    u8 r, g, b, a;
} Color32;

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

extern u8 gEngineApp[];

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
        CEngineApp__DrawFilledRect(gEngineApp, &color, scr->m_X, scr->m_Y,
                      scr->m_X + scr->m_Width, scr->m_Y + scr->m_Height);
    }
}
