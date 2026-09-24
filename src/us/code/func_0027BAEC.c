#include "common.h"

extern s32 D_800F56A8;

f32 func_0027BAEC(void) {
    switch (D_800F56A8) {
        case 5:
            return 6.0f;
        case 4:
            return 8.0f;
        case 1:
            return 12.0f;
        case 0:
            return 24.0f;
        case 3:
            return 16.0f;
        default:
            return 1.0f;
    }
}
