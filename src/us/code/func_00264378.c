#include "common.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/scene.h"

typedef struct {
    u8 pad0[0x8];
    void (*onEnter)(void *actor, void *state); /* 0x8 */
} StateHooks;

void Platform_UpdateSpecial(void *actor, void *state);
void Platform_UpdatePosition(void *actor, void *state);

#ifdef TARGET_PORT
#include "structs/game_object.h"

/* The port: the same on CGameObjectInstance/CAI (N64 offsets: def 0x14, flags 0xD4, the AI's
 * current mode row 0x30 and its enter hook at row + 8, m_VarF32[6] at 0x124). The
 * intelligence body (+0x14, asset data) keeps its byte offset 0x48 (the spin speed). */
void Platform_Step(u8 *actor_, u8 *state_) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;
    CAI *state = (CAI *) state_;
    u8 *info = (u8 *) actor->header.m_pIntelligence + 0x14;
    CAIModeInfo *hooks;

    if (CScene__GetInstanceSavedBit(&gScene, actor) == 0) {
        actor->m_dwFlags &= ~0x10000;
        actor->m_dwFlags &= ~0x100;
        return;
    }
    actor->m_dwFlags |= 0x10000;
    hooks = state->m_pModeCurrent;
    if (hooks != NULL && hooks->m_pfCallback != NULL) {
        ((void (*)(void *, void *)) hooks->m_pfCallback)(actor, state);
    }
    state->m_VarF32[6] += *(f32 *) (info + 0x48) * gFrameIncrement;
    Platform_UpdateSpecial(actor, state);
    Platform_UpdatePosition(actor, state);
}
#else
void Platform_Step(u8 *actor, u8 *state) {
    u8 *info = *(u8 **) (actor + 0x14) + 0x14;
    StateHooks *hooks;

    if (CScene__GetInstanceSavedBit(&gScene, actor) == 0) {
        *(s32 *) (actor + 0xD4) &= ~0x10000;
        *(s32 *) (actor + 0xD4) &= ~0x100;
        return;
    }
    *(s32 *) (actor + 0xD4) |= 0x10000;
    hooks = *(StateHooks **) (state + 0x30);
    if (hooks != NULL && hooks->onEnter != NULL) {
        hooks->onEnter(actor, state);
    }
    *(f32 *) (state + 0x124) += *(f32 *) (info + 0x48) * gFrameIncrement;
    Platform_UpdateSpecial(actor, state);
    Platform_UpdatePosition(actor, state);
}
#endif
