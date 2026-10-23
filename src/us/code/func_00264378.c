#include "common.h"

typedef struct {
    u8 pad0[0x8];
    void (*onEnter)(void *actor, void *state); /* 0x8 */
} StateHooks;

extern u8 D_800F7078[];
extern f32 D_800B6D28; /* frame delta */
s32 func_00220408(void *table, void *actor);
void func_00263A20(void *actor, void *state);
void func_00263DD8(void *actor, void *state);

void func_00264378(u8 *actor, u8 *state) {
    u8 *info = *(u8 **) (actor + 0x14) + 0x14;
    StateHooks *hooks;

    if (func_00220408(D_800F7078, actor) == 0) {
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
    func_00263A20(actor, state);
    func_00263DD8(actor, state);
}
