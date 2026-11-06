#include "common.h"
#include "m2c_macros.h"

extern s32 D_801309C0;

void PakMessage_InitCannotSave(void) {
    D_801309C0 = 6;
}
