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
