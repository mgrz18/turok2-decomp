#include "common.h"

s32 func_00268160(u8 *arg0) {
    if (arg0[0] == 1 && (*(s32 *)(arg0 + 0xD4) & 0x300000)) {
        return 1;
    }
    switch (**(s32 **)(arg0 + 0x14)) {
        case 1:
        case 2:
        case 5:
        case 8:
        case 9:
            return 1;
    }
    return 0;
}
