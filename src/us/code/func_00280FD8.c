#include "common.h"
#include "structs/camera.h"
#include "structs/game.h"

void CCamera__UpdateFlash();                            /* extern */
void CEngineApp__DrawFilledRect(void *, void *, s32, s32, s32, s32); /* extern */

/* Draws the camera's flash: a rectangle of m_FlashColor over its viewport. */
void CCamera__DrawFlash(CCamera *cam) {
    f32 x;

    CCamera__UpdateFlash();
    if (cam->m_FlashMode != 0) {
        x = cam->m_Viewport.m_ScreenPosX;
        CEngineApp__DrawFilledRect(&gEngineApp, cam->m_FlashColor, (s32) x, (s32) cam->m_Viewport.m_ScreenPosY,
                      (s32) (x + cam->m_Viewport.m_Width),
                      (s32) (cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height));
    }
}
