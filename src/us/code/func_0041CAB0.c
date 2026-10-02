#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800B5C60;
extern M2C_UNK D_800B5C6C;
extern s32 D_8011ACE4;

s32 func_0041CAB0(void *arg0) {
    if (D_8011ACE4 != 0) {
        M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800B5C6C;
    } else {
        M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800B5C60;
    }
    return 0;
}
