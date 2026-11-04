#include "structs/weapon.h"

s32 func_002572FC(s16 id, s32 mode) {
    switch (mode) {
    case 0:
        if (D_800AF618[id]->flags & 1) {
            return 1;
        }
        break;
    case 1:
        if (D_800AF618[id]->flags & 2) {
            return 1;
        }
        break;
    case 2:
        if (id == 0x13) {
            return 1;
        }
        break;
    }
    return 0;
}
