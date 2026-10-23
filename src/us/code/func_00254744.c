#include "common.h"
#include "m2c_macros.h"

extern u8 D_8011AAF1;

void func_00254744(void *arg0) {
    if (D_8011AAF1 != 0) {
        M2C_FIELD(arg0, s16 *, 0x52A) = 1;
        return;
    }
    M2C_FIELD(arg0, s16 *, 0x52A) = 3;
}
