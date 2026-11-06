#include "common.h"

extern u8 gScene[];
s32 CScene__GetInstanceSavedBit(void *table, void *actor);
s32 CAI__SetMode(void *actor, void *anim, s32 mode);

void Turret_Advance(u8 *actor, u8 *anim) {
    u8 *def;

    def = *(u8 **) (actor + 0x14);
    *(s32 *) (anim + 0x10C) = 0;
    *(s32 *) (anim + 0x110) = 0;
    *(f32 *) (anim + 0x64) = *(f32 *) (def + 0x7C);
    if (CScene__GetInstanceSavedBit(gScene, actor) == 0) {
        if (*(s32 *) (def + 0x14) & 0x1000) {
            *(s32 *) (actor + 0xD4) &= ~0x2000;
            *(s32 *) (actor + 0xD4) &= ~0x100;
        }
        if (*(s32 *) (def + 0x14) & 0x800) {
            *(s32 *) (actor + 0xD4) &= ~0x100;
            CAI__SetMode(actor, anim, 0);
        } else {
            CAI__SetMode(actor, anim, 1);
        }
    } else {
        CAI__SetMode(actor, anim, 0x40);
    }
    anim[0x37] = 0;
}
