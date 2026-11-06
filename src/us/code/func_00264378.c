#include "common.h"

typedef struct {
    u8 pad0[0x8];
    void (*onEnter)(void *actor, void *state); /* 0x8 */
} StateHooks;

extern u8 D_800F7078[];
extern f32 D_800B6D28; /* frame delta */
s32 CScene__GetInstanceSavedBit(void *table, void *actor);
void Platform_UpdateSpecial(void *actor, void *state);
void Platform_UpdatePosition(void *actor, void *state);

void Platform_Step(u8 *actor, u8 *state) {
    u8 *info = *(u8 **) (actor + 0x14) + 0x14;
    StateHooks *hooks;

    if (CScene__GetInstanceSavedBit(D_800F7078, actor) == 0) {
        *(s32 *) (actor + 0xD4) &= ~0x10000;
        *(s32 *) (actor + 0xD4) &= ~0x100;
        return;
    }
    *(s32 *) (actor + 0xD4) |= 0x10000;
    hooks = *(StateHooks **) (state + 0x30);
    if (hooks != NULL && hooks->onEnter != NULL) {
        hooks->onEnter(actor, state);
    }
    *(f32 *) (state + 0x124) += *(f32 *) (info + 0x48) * D_800B6D28;
    Platform_UpdateSpecial(actor, state);
    Platform_UpdatePosition(actor, state);
}
