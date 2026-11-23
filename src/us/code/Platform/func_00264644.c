/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00207C94.c, func_00207C94, with Turok 2's layouts. */
#include "common.h"
#include "protos/ai.h"
#include "structs/game_object.h"

/* The platform intelligence's flags and m_ReturnDelayTime; the AI's mode time. */
void Platform_AtEndUpdate(CGameObjectInstance *actor, CAI *state) {
    CPlatformIntelligence *info = &actor->header.m_pIntelligence->m_Platform;
    s32 ok = 1;

    if (info->m_dwPlatformFlags & 0x20) {
        u32 f = state->m_dwFlags & 0x10000000;
        ok = f != 0;
    }
    if (info->m_dwPlatformFlags & 0x200) {
        if (!(state->m_dwFlags & 0x20000)) {
            ok = 0;
        }
    }
    if (info->m_dwPlatformFlags & 0x800) {
        if (state->m_dwFlags & 0x20000) {
            ok = 0;
        }
    }
    if (ok != 0) {
        if (state->m_ModeTime >= info->m_ReturnDelayTime) {
            CAI__SetMode(actor, state, 3);
        }
    } else {
        state->m_ModeTime = 0.0f;
    }
}
