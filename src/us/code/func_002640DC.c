#include "common.h"

s32 func_002640DC(s32 arg0, void *arg1) {
    switch (*(s8 *)((u8 *)arg1 + 0x37)) {
        case 3:
            return 0x5280;
        case 1:
        case 2:
            return 0x5294;
        case 0:
            switch (*(s8 *)((u8 *)arg1 + 0x34)) {
                case 0:
                case 2:
                    return 0x526C;
                case 1:
                case 3:
                    return 0x5276;
            }
            break;
        case 4:
            return 0x528A;
        default:
            return 0x526C;
    }
    return 0x526C;
}
