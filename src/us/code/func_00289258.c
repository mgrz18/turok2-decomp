#include "common.h"
#include "m2c_macros.h"

extern s32 func_00286E14(s32, s32, void *, s32, s32, s32, f32, f32);
extern void func_0028908C(void);
extern f32 D_800A9DF8;
extern u8 D_800B3634[];
extern s32 D_800B6D64;
extern s32 D_800C2030;
extern s32 D_8011F13C;

void func_00289258(s32 arg0) {
    if (D_800B6D64 != 0) {
        if (D_8011F13C != 0) {
            func_00286E14(arg0, 1, D_800B3634, D_800C2030 / 2, 0x5A, 0xA, D_800A9DF8, D_800A9DF8);
            return;
        }
        func_0028908C();
    }
}
