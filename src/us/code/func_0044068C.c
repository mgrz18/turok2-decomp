#include "common.h"
#include "m2c_macros.h"
#include "globals/boss.h"

extern M2C_UNK D_800C0530;

void Primagen_EnterMode23_2(void *arg0) {
    M2C_FIELD(arg0, f32 *, 0x1FC) = (f32) M2C_FIELD(&D_800C0530, f32 *, 4);
    gLevelScript.timer = M2C_FIELD(&D_800C0530, f32 *, 4);
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2000);
}
