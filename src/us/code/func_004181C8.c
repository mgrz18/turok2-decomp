#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00285A68(s8);                          /* extern */
extern s32 D_800C2020;

void func_004181C8(void *arg0) {
    func_00285A68(M2C_FIELD(M2C_FIELD(arg0, void **, 0x20), s8 *, 4));
    D_800C2020 = 0;
}
