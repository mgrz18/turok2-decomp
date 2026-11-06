#include "common.h"

extern s32 D_80130C60[];
extern s32 D_80130C70[];

s32 Pak_GetStatus(s32 i) {
    if (D_80130C60[i] != 3) {
        return -2;
    }
    return D_80130C70[i];
}
