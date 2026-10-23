#include "common.h"

f32 func_00268460(u8 *actor) {
    u8 *def = *(u8 **) (actor + 0x14);
    u8 *weapon;

    switch (*(s32 *) def) {
        case 11:
            if (actor[0] == 1 && (*(s32 *) (actor + 0xD4) & 0x300000)) {
                weapon = *(u8 **) (*(u8 **) (actor + 0x1A8) + 0xBB0);
                if (weapon != NULL) {
                    return func_00268460(weapon);
                }
            }
            return *(f32 *) (*(u8 **) (actor + 0x14) + 0x2C);
        case 1:
        case 4:
            return *(f32 *) (def + 0x2C);
        case 5:
            return *(f32 *) (def + 0x18);
    }
    return 0.0f;
}
