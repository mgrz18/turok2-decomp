#include "common.h"
#include "m2c_macros.h"

extern s32 D_80130964;
extern s32 D_8013099C;

void DamagedPakMenu_InitInGame_2(void) {
    D_8013099C = 0;
    D_80130964 = 1;
}
