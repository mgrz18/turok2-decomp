#include "common.h"
#include "structs/camera.h"

/* A view of the globals from D_8011AAE8: the HUD color byte and, as
 * D_8011ACC4, the time left in the match (in ticks of 1/15 s). */
typedef struct {
    /* 0x000 */ u8 pad0[2];
    /* 0x002 */ u8 color;           /* D_8011AAEA */
    /* 0x003 */ u8 pad3[0x1DC - 3];
    /* 0x1DC */ f32 timeLeft;       /* D_8011ACC4 */
} HudGlobals;

extern HudGlobals D_8011AAE8;
extern u32 D_800B6D20;          /* frame counter */
extern s32 D_801198E0;
extern s32 D_800C2030;          /* screen width */
extern s32 D_800C2034;          /* screen height */

extern void func_00279028(s32 color);
extern void func_00277CF0(s32 value, f32 x, f32 y, f32 sx, f32 sy, s32 arg5, s32 arg6, s32 arg7);

/* Draw the match clock, minutes and seconds, centered at the top of the
 * screen (mid-screen when D_801198E0 != 1), scaled to cam's viewport; in the
 * last 30 seconds it blinks. */
void func_00252E08(s32 arg0, CCamera *cam) {
    f32 secs, mins;
    f32 sx, sy;
    f32 x, y;

    func_00279028(D_8011AAE8.color);
    if (D_8011AAE8.timeLeft <= 0.0f) {
        return;
    }
    secs = D_8011AAE8.timeLeft * (1.0f / 15);
    mins = (s32)(secs * (1.0f / 60));
    secs -= mins * 60.0f;
    sx = cam->m_Viewport.m_Width / D_800C2030;
    sy = cam->m_Viewport.m_Height / D_800C2034;
    if (mins < 1.0f && secs < 30.0f && D_800B6D20 % 15 < 5) {
        return;
    }
    if (D_801198E0 == 1) {
        x = D_800C2030 * 0.5f;
        y = 20.0f;
    } else {
        x = D_800C2030 * 0.5f;
        y = (D_800C2034 - 10) * 0.5f;
    }
    func_00277CF0((s32)mins, x - sx * 6.0f, y, sx, sy, 1, 1, 0);
    func_00277CF0((s32)secs, x + sx * 2.0f, y, sx, sy, 1, 0, 2);
}
