#include "common.h"

f32 func_002683CC(u8 *actor) {
    u8 *def = *(u8 **) (actor + 0x14);

    switch (*(s32 *) def) {
        case 1:
        case 4:
        case 11:
            return *(f32 *) (def + 0x34);
    }
    return 0.0f;
}
