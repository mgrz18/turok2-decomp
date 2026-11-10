/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00207ABC.c, func_00207ABC, with Turok 2's layouts. */
#include "common.h"
#include "protos/ai.h"

typedef struct {
    u8 pad0[0x24];
    u32 flags; /* 0x24 */
    u8 pad28[0x24];
    f32 delay; /* 0x4C */
} StateInfo;

typedef struct {
    u32 flags; /* 0x0 */
    u8 pad4[0x3C];
    f32 timer; /* 0x40 */
} MotionState;


#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port: the intelligence from CGameObjectInstance (N64 0x14), the AI's m_ModeTime (N64 0x40). */
void Platform_AtStartUpdate(u8 *actor_, MotionState *state_) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;
    CAI *state = (CAI *) state_;
    StateInfo *info = (StateInfo *) ((u8 *) actor->header.m_pIntelligence + 0x14);
    s32 ok = 1;

    if (info->flags & 0x10) {
        u32 f = state->m_dwFlags & 0x8000000;
        ok = f != 0;
    }
    if (info->flags & 0x100) {
        if (!(state->m_dwFlags & 0x20000)) {
            ok = 0;
        }
    }
    if (info->flags & 0x400) {
        if (state->m_dwFlags & 0x20000) {
            ok = 0;
        }
    }
    if (ok != 0) {
        if (state->m_ModeTime >= info->delay) {
            CAI__SetMode(actor, state, 1);
        }
    } else {
        state->m_ModeTime = 0.0f;
    }
}
#else
void Platform_AtStartUpdate(u8 *actor, MotionState *state) {
    StateInfo *info = (StateInfo *) (*(u8 **) (actor + 0x14) + 0x14);
    s32 ok = 1;

    if (info->flags & 0x10) {
        u32 f = state->flags & 0x8000000;
        ok = f != 0;
    }
    if (info->flags & 0x100) {
        if (!(state->flags & 0x20000)) {
            ok = 0;
        }
    }
    if (info->flags & 0x400) {
        if (state->flags & 0x20000) {
            ok = 0;
        }
    }
    if (ok != 0) {
        if (state->timer >= info->delay) {
            CAI__SetMode(actor, state, 1);
        }
    } else {
        state->timer = 0.0f;
    }
}
#endif
