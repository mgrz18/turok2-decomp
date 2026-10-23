#include "common.h"

extern u8 D_800F7078[];
s32 func_00220408(void *table, void *actor);
s32 func_00243414(void *actor, void *anim, s32 mode);

void func_002626E8(u8 *actor, u8 *anim) {
    u8 *def;

    def = *(u8 **) (actor + 0x14);
    *(s32 *) (anim + 0x10C) = 0;
    *(s32 *) (anim + 0x110) = 0;
    *(f32 *) (anim + 0x64) = *(f32 *) (def + 0x7C);
    if (func_00220408(D_800F7078, actor) == 0) {
        if (*(s32 *) (def + 0x14) & 0x1000) {
            *(s32 *) (actor + 0xD4) &= ~0x2000;
            *(s32 *) (actor + 0xD4) &= ~0x100;
        }
        if (*(s32 *) (def + 0x14) & 0x800) {
            *(s32 *) (actor + 0xD4) &= ~0x100;
            func_00243414(actor, anim, 0);
        } else {
            func_00243414(actor, anim, 1);
        }
    } else {
        func_00243414(actor, anim, 0x40);
    }
    anim[0x37] = 0;
}
