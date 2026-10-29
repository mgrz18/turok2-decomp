/* func_00277090 and func_00277328, one source file: splat's func_00277088
 * started with the two junk words before this object (versions/cuts.us.txt),
 * and the 1.0f at 0x800A8240 that opens the file's .rodata is
 * func_00277090's. */
#include "common.h"
#include "m2c_macros.h"
#include "structs/player.h"

extern s32 Cinema_PathPlaying(void);
extern void func_0027AEA8(s32 id, s32 frame, s32 *width, s32 *height); /* HUD sprite size */
extern void func_0027AF4C(void);
extern void func_00277328(COverlay *ov);                                /* runs the overlay script */
extern void func_0027AFAC(s32 x0, s32 y0, s32 x1, s32 y1, u8 r, u8 g, u8 b, u8 a); /* filled rect */
extern void func_00279028(u8 alpha);
extern void func_0027B320(s32 id, s32 frame, s16 x, s16 y, f32 scaleX, f32 scaleY, s32 flags);

extern s32 D_8011AC88;      /* game mode */
extern u8 D_8011AAEA;       /* HUD opacity */
extern s32 D_800C2030;      /* screen width */

/* Draws a boss health bar (1: m_BossOverlay, green; 2: m_BossOverlay2, red):
 * the frame sprite 0x191 and a bar whose length eases toward the boss percent. */
void func_00277090(COnScreen *os, s32 which) {
    s32 width;
    s32 height;
    COverlay *ov;
    f32 percent;
    s32 x;
    s32 y;
    s32 r;
    s32 g;
    s32 b;
    s32 yOff;
    u8 *alpha;
    s32 left;

    if (Cinema_PathPlaying() != 0 || D_8011AC88 != 13) {
        return;
    }
    func_0027AEA8(0x191, 0, &width, &height);
    if (width == 0 || height == 0) {
        return;
    }
    switch (which) {
        case 1:
            ov = &os->m_BossOverlay;
            percent = os->m_BossPercent;
            r = 0x32;
            g = 0xFF;
            b = 0x32;
            yOff = 8;
            break;
        case 2:
            ov = &os->m_BossOverlay2;
            percent = os->m_BossPercent2;
            r = 0xFF;
            g = 0x32;
            b = 0x32;
            yOff = 0x15;
            break;
        default:
            return;
    }
    x = ov->m_PositionX + (f32)((D_800C2030 - width) / 2);
    y = ov->m_PositionY + (f32)yOff;
    func_0027AF4C();
    func_00277328(ov);
    if (ov->m_Mode == 1) {
        if (ov->m_Value < percent) {
            ov->m_Value += 8;
            if (percent < ov->m_Value) {
                ov->m_Value = percent;
            }
        } else if (percent < ov->m_Value) {
            ov->m_Value -= 4;
            if (ov->m_Value < percent) {
                ov->m_Value = percent;
            }
        }
    }
    alpha = &D_8011AAEA;
    left = x + 0x1E;
    func_0027AFAC(left, y + 8, left + (width - 0x3C) * ov->m_Value / 100, y + 0xE,
                  r, g, b, *alpha * 200 / 255);
    func_00279028(*alpha);
    func_0027B320(0x191, 0, x, y, 1.0f, 1.0f, 0);
}

extern s32 D_800C2030;      /* screen width */
extern s32 D_800C2034;      /* screen height */


/* Runs a HUD overlay's script (docs/HUD.md) and the motion it started, one
 * step a frame. Ops: 0 stop, 1 set position, 2 wait, 3 slide to, 4 fall in
 * Y, 5 fall in X, 6 end. Script values are whole numbers; the fall
 * accelerations are 16.16 fixed point. An op that takes no time runs the
 * next one at once. */
void func_00277328(COverlay *ov) {
    s32 next;
    f32 dx;
    f32 dy;
    s32 bottom = D_800C2034 + 0x60;
    s32 right = D_800C2030 + 0x60;

    do {
        next = 0;
        switch (ov->m_Mode) {
            case 0:
            case 1:
                break;
            case 2:
                if (ov->m_pScript == NULL) {
                    goto no_script;
                }
                switch (*ov->m_pScript++) {
                    case 6:
                        ov->m_Mode = 0;
                        ov->m_pScript = NULL;
                        break;
                    case 1:
                        next = 1;
                        ov->m_PositionX = *ov->m_pScript++;
                        ov->m_PositionY = *ov->m_pScript++;
                        break;
                    case 2:
                        ov->m_Mode = 3;
                        ov->m_Time = *ov->m_pScript++;
                        break;
                    case 3:
                        ov->m_bOnScreen = 1;
                        ov->m_Mode = 4;
                        ov->m_FinalX = *ov->m_pScript++;
                        ov->m_FinalY = *ov->m_pScript++;
                        ov->m_ZoomFactor = 1.0f / *ov->m_pScript++;
                        break;
                    case 4:
                        ov->m_bOnScreen = 1;
                        ov->m_Mode = 5;
                        ov->m_VelY = 0.0f;
                        ov->m_AccelY = *ov->m_pScript++ * (1.0f / 65536.0f);
                        break;
                    case 5:
                        ov->m_bOnScreen = 1;
                        ov->m_Mode = 6;
                        ov->m_VelX = 0.0f;
                        ov->m_AccelX = *ov->m_pScript++ * (1.0f / 65536.0f);
                        break;
                    default:
                        ov->m_Mode = 1;
                        ov->m_pScript = NULL;
                        break;
                }
                break;
            case 3:
                if (ov->m_Time-- <= 0) {
                    next = 1;
                }
                break;
            case 4:
                dx = (ov->m_FinalX - ov->m_PositionX) * ov->m_ZoomFactor;
                dy = (ov->m_FinalY - ov->m_PositionY) * ov->m_ZoomFactor;
                ov->m_PositionX += dx;
                ov->m_PositionY += dy;
                if (((dx < 0.0f) ? -dx : dx) < 0.25f && ((dy < 0.0f) ? -dy : dy) < 0.25f) {
                    ov->m_PositionX = ov->m_FinalX;
                    ov->m_PositionY = ov->m_FinalY;
                    next = 1;
                }
                break;
            case 5:
                ov->m_PositionY += ov->m_VelY;
                ov->m_VelY += ov->m_AccelY;
                if (bottom < ov->m_PositionY || ov->m_PositionY < -90.0f) {
                    ov->m_bOnScreen = 0;
                    next = 1;
                }
                break;
            case 6:
                ov->m_PositionX += ov->m_VelX;
                ov->m_VelX += ov->m_AccelX;
                if (right < ov->m_PositionX || ov->m_PositionX < -90.0f) {
                    ov->m_bOnScreen = 0;
                    next = 1;
                }
                break;
            no_script:              /* here, not inline in case 2: the ROM's order */
                ov->m_Mode = 1;
                break;
        }
        if (!next) {
            return;
        }
        ov->m_Mode = 2;
    } while (1);
}
