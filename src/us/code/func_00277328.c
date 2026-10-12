#include "common.h"
#include "m2c_macros.h"
#include "structs/player.h"

extern s32 D_800C2030;      /* screen width */
extern s32 D_800C2034;      /* screen height */

/* A float of func_00277090, the function before this one in the same source
 * file: it opens the file's .rodata, so it is here until that is matched. */
const f32 D_800A8240 = 1.0f;

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
