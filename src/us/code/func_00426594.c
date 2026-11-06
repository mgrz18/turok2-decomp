#include "common.h"

extern s32 D_80130C60[];        /* per controller-pak slot status */

s32 Pak_IsRumblePak(s32 arg0) {
    return D_80130C60[arg0] == 2;
}
