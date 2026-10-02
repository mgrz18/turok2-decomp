#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_80130C60;

s32 func_00426594(s32 arg0) {
    return *(((__typeof__(&D_80130C60))((s8 *)&D_80130C60 + (arg0 * 4)))) == 2;
}
