/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00207ABC.c, func_00207ABC, with Turok 2's layouts. */
#include "common.h"
#include "protos/ai.h"
#include "structs/game_object.h"

/* The platform intelligence's flags and m_GoDelayTime; the AI's mode time. */
void Platform_AtStartUpdate(CGameObjectInstance *actor, CAI *state) {
    CPlatformIntelligence *info = &actor->header.m_pIntelligence->m_Platform;
    s32 ok = 1;

    if (info->m_dwPlatformFlags & 0x10) {
        u32 f = state->m_dwFlags & 0x8000000;
        ok = f != 0;
    }
    if (info->m_dwPlatformFlags & 0x100) {
        if (!(state->m_dwFlags & 0x20000)) {
            ok = 0;
        }
    }
    if (info->m_dwPlatformFlags & 0x400) {
        if (state->m_dwFlags & 0x20000) {
            ok = 0;
        }
    }
    if (ok != 0) {
        if (state->m_ModeTime >= info->m_GoDelayTime) {
            CAI__SetMode(actor, state, 1);
        }
    } else {
        state->m_ModeTime = 0.0f;
    }
}
