#include "common.h"

extern s32 D_80130C60[];
extern s32 D_80130C70[];

s32 func_00426BD8(s32 i) {
    if (D_80130C60[i] != 3) {
        return 0;
    }
    return D_80130C70[i] == -4;
}
