#include "common.h"

extern u8 D_800F7078[];
s32 func_00220408(void *table, void *actor);
s32 func_00243414(void *actor, void *state, s32 mode);

void func_0026613C(u8 *actor, u8 *state) {
    if (func_00220408(D_800F7078, actor) == 1) {
        func_00243414(actor, state, 1);
        switch (*(u16 *) (actor + 0xB8)) {
            case 0x64C:
                *(s32 *) (actor + 0xD4) &= ~0x2000;
                *(s32 *) (actor + 0xD4) &= ~0x100;
                break;
            case 0x64B:
            case 0x64E:
                *(s32 *) (actor + 0xD4) &= ~0x8000000;
                break;
        }
    } else {
        func_00243414(actor, state, 0);
    }
}
