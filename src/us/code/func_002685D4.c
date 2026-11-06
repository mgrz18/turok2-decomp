#include "common.h"

s32 GameObject_GetInstanceBelow(u8 *arg0) {
    if (arg0[0] == 1) {
        return *(s32 *)(arg0 + 0x30);
    }
    return 0;
}
