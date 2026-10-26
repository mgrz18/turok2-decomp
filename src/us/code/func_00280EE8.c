#include "common.h"

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

extern f32 D_800B6D28;          /* frame time scale */
extern u8 D_800F6CB0[];

void func_002883E0(void *, Color32 *, s32, s32, s32, s32);

/* Fades out a red flash over the screen area and draws it while it lasts. */
void func_00280EE8(Screen *scr) {
    Color32 color;

    if (scr->m_FlashAlpha > scr->m_FlashFade) {
        scr->m_FlashAlpha -= (u32)(scr->m_FlashFade * D_800B6D28);
    } else {
        scr->m_FlashAlpha = 0;
    }
    if (scr->m_FlashAlpha != 0) {
        color.r = 0xFF;
        color.g = 0;
        color.b = 0;
        color.a = scr->m_FlashAlpha;
        func_002883E0(D_800F6CB0, &color, scr->m_X, scr->m_Y,
                      scr->m_X + scr->m_Width, scr->m_Y + scr->m_Height);
    }
}
