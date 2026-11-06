#include "common.h"
#include "structs/camera.h"

extern s32 gScreenWidth;      /* screen width */
extern s32 gScreenHeight;      /* screen height */
extern u8 gEngineApp_m_Settings_multiplayer;

void CCamera__WorldToScreen(CCamera *, f32 *, f32 *, f32 *);
void Hud_DrawSprite(s32, s32, s16, s16, f32, f32, s32);

/* Draws a sprite at a world position projected to the screen (or centred
 * for the HUD sprites 0x2BC, 0x2DA and 0x2E4). */
void Hud_DrawWorldSprite(CCamera *view, f32 *pos, s32 sprite, f32 frame, f32 scale, f32 size) {
    f32 x;
    f32 y;
    f32 half;

    if (sprite == 0x2DA || sprite == 0x2E4 || sprite == 0x2BC) {
        x = view->m_Viewport.m_ScreenPosX + view->m_Viewport.m_Width * 0.5f;
        y = view->m_Viewport.m_ScreenPosY + view->m_Viewport.m_Height * 0.5f;
    } else {
        /* the last view-volume plane: points in front have dot <= dist */
        if (!(view->m_ViewVolume[5].m_vNormal.x * pos[0] + view->m_ViewVolume[5].m_vNormal.y * pos[1] + view->m_ViewVolume[5].m_vNormal.z * pos[2] <= view->m_ViewVolume[5].m_Dist)) {
            return;
        }
        CCamera__WorldToScreen(view, pos, &x, &y);
        if (x < -64.0f || (f32) (gScreenWidth + 64) < x || y < -64.0f || (f32) (gScreenHeight + 64) < y) {
            return;
        }
        if (gEngineApp_m_Settings_multiplayer != 0) {
            x = view->m_Viewport.m_ScreenPosX + x / gScreenWidth * view->m_Viewport.m_Width;
            y = view->m_Viewport.m_ScreenPosY + y / gScreenHeight * view->m_Viewport.m_Height;
        }
    }
    half = scale * size - 1.0f;
    Hud_DrawSprite(sprite, frame, x - half, y - half, size, size, 1);
}
