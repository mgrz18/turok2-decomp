#include "common.h"
#include "m2c_macros.h"

extern s32 D_800C202C;
extern s32 D_8011B108;

s32 func_00421880(void *arg0) {
    if ((M2C_FIELD(M2C_FIELD(arg0, void **, 0x20), s32 *, 0xB0) & 0x1000) && (D_800C202C == 0)) {
        D_8011B108 = 1;
        D_800C202C = 1;
    }
    return D_8011B108;
}
