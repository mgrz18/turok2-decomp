#include "common.h"
#include "structs/game.h"
#include "structs/camera.h"
#include "globals/game.h"
#include "globals/camera.h"
#include "globals/render.h"

extern u32 gFrameCountGameplay;          /* frame counter */

extern void Hud_SetupTextureMode(s32 color);
extern void Hud_DrawNumber(s32 value, f32 x, f32 y, f32 sx, f32 sy, s32 arg5, s32 arg6, s32 arg7);

/* Draw the match clock (gEngineApp_m_Settings.match.timeLeft, in ticks of 1/15 s), minutes and seconds, centered at the top of the
 * screen (mid-screen when gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount != 1), scaled to cam's viewport; in the
 * last 30 seconds it blinks. */
void Hud_DrawMatchClock(s32 arg0, CCamera *cam) {
    f32 secs, mins;
    f32 sx, sy;
    f32 x, y;

    Hud_SetupTextureMode(gEngineApp_m_Settings.hudAlpha);
    if (gEngineApp_m_Settings.match.timeLeft <= 0.0f) {
        return;
    }
    secs = gEngineApp_m_Settings.match.timeLeft * (1.0f / 15);
    mins = (s32)(secs * (1.0f / 60));
    secs -= mins * 60.0f;
    sx = cam->m_Viewport.m_Width / gScreenWidth;
    sy = cam->m_Viewport.m_Height / gScreenHeight;
    if (mins < 1.0f && secs < 30.0f && gFrameCountGameplay % 15 < 5) {
        return;
    }
    if (gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_nCount == 1) {
        x = gScreenWidth * 0.5f;
        y = 20.0f;
    } else {
        x = gScreenWidth * 0.5f;
        y = (gScreenHeight - 10) * 0.5f;
    }
    Hud_DrawNumber((s32)mins, x - sx * 6.0f, y, sx, sy, 1, 1, 0);
    Hud_DrawNumber((s32)secs, x + sx * 2.0f, y, sx, sy, 1, 0, 2);
}
