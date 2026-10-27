#include "common.h"

s32 func_00268C04(u8 *obj); /* the object's type id */

/* Maps an object's type id to a class 0-9 (3 by default). */
s32 func_00245724(u8 *obj) {
    switch (func_00268C04(obj)) {
    case 1: case 2:
        return 0;
    case 3: case 4:
        return 1;
    case 5: case 6:
        return 2;
    case 7: case 8:
        return 3;
    case 9: case 10:
    case 100:
    case 102:
    case 0x460: case 0x461: case 0x462: case 0x463:
        return 4;
    case 11: case 12:
        return 5;
    case 13: case 14:
        return 6;
    case 15: case 16:
        return 7;
    case 17: case 18:
        return 8;
    case 19: case 20:
        return 9;
    default:
        return 3;
    }
}
