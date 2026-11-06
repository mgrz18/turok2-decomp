#include "common.h"
#include "m2c_macros.h"

extern s8 D_800F1F94;

void Throwable_IdleUpdate(void) {
    D_800F1F94 = 2;
}
