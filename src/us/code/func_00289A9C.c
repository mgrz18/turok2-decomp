#include "common.h"
#include "m2c_macros.h"

s32 func_00412438(s32);
s32 func_0042C700(s32, s32, s32);

void *func_002532A8(M2C_UNK *);                     /* extern */
M2C_UNK func_00288DD0(s32, M2C_UNK);                /* extern */
extern s32 D_800B6D70;
extern s32 D_800B6D74;
extern s32 D_800C2034;
extern s32 D_800F6CA0;
extern M2C_UNK D_800F7078;
extern M2C_UNK D_80119870;
extern s32 D_8011AD00;
extern f32 D_8011F120;

void func_00289A9C(s32 arg0) {
    void *temp_v0;

    D_800B6D70 = 0;
    D_800F6CA0 = 0;
    D_8011F120 = (f32) D_800C2034;
    D_8011AD00 = 0;
    temp_v0 = func_002532A8(&D_80119870);
    if ((temp_v0 != NULL) && (M2C_FIELD(temp_v0, s32 *, 0x9F0) != 0)) {
        D_800B6D74 = 1;
    } else {
        D_800B6D74 = 0;
    }
    if (D_800B6D74 != 0) {
        func_0042C700((s32) &D_800F7078, ~func_00412438(0x82), 0);
        return;
    }
    func_00288DD0(arg0, 0x82);
}
