#include "common.h"

s32 func_002682CC(void *arg0) {
    switch (**(s32 **)((u8 *)arg0 + 0x14)) {
        case 0:
        case 12:
        default:
            return 1;
        case 1:
        case 4:
        case 9:
        case 11:
            return 0;
    }
}
