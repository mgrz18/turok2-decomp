/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002935A4.c, func_002935A4, with Turok 2's layouts. */
#include "common.h"
#include "structs/game.h"

extern f32 gFrameIncrement; /* frame delta */

void CEngineApp__UpdateFade(CEngineApp *g) {
    f32 speed;
    f32 kk;

    if (g->m_Settings.fadeMode != 0) {
        speed = 0.1f;
        if (g->m_Settings.fastFade != 0) {
            /* Redundant branches kept: they split the blocks so fadeState is reloaded. */
            if (g || gFrameIncrement) {
                speed = 0.5f;
            } else {
                speed = 0.5f;
            }
        }
        if (g) {
            kk = speed;
        } else {
            kk = speed;
        }
        if (g->m_Settings.fadeMode != 1) {
            if (g->m_Settings.fadeMode == 2) {
                if (0.0f < g->m_Settings.fade) {
                    g->m_Settings.fade = g->m_Settings.fade - (kk * gFrameIncrement);
                    if (g->m_Settings.fade < 0.0f) {
                        g->m_Settings.fade = 0.0f;
                    }
                } else {
                    g->m_Settings.fade = g->m_Settings.fade - 1.0f;
                }
                if (g->m_Settings.fade < -4.0f) {
                    s32 old = g->m_Settings.mode;
                    g->m_Settings.mode = 0x11;
                    g->m_Settings.fade = 0.0f;
                    g->m_Settings.fadeMode = 1;
                    g->m_Settings.prevMode = old;
                }
            }
        } else {
            g->m_Settings.fade = g->m_Settings.fade + (speed * ((gFrameIncrement < 0.5f) ? 0.5f : gFrameIncrement));
            if (1.0f <= g->m_Settings.fade) {
                g->m_Settings.fadeMode = 0;
                g->m_Settings.fade = 1.0f;
                g->m_Settings.fastFade = 0;
            }
        }
    }
}
