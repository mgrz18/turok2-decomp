#include "common.h"
#include "structs/camera.h"
#include "structs/game.h"
#include "globals/engine.h"

typedef struct {
    u8 r, g, b, a;
} Color32;


void CEngineApp__DrawFilledRect(void *, Color32 *, s32, s32, s32, s32);

/* Fades out a red flash over the screen area and draws it while it lasts. */
void CCamera__DrawHitFlash(CCamera *scr) {
    Color32 color;

    if (scr->m_Flash > scr->m_FlashDec) {
        scr->m_Flash -= (u32)(scr->m_FlashDec * gFrameIncrement);
    } else {
        scr->m_Flash = 0;
    }
    if (scr->m_Flash != 0) {
        color.r = 0xFF;
        color.g = 0;
        color.b = 0;
        color.a = scr->m_Flash;
        CEngineApp__DrawFilledRect(&gEngineApp, &color, scr->m_Viewport.m_ScreenPosX, scr->m_Viewport.m_ScreenPosY,
                      scr->m_Viewport.m_ScreenPosX + scr->m_Viewport.m_Width, scr->m_Viewport.m_ScreenPosY + scr->m_Viewport.m_Height);
    }
}
