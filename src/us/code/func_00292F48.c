#include "common.h"
#include "m2c_macros.h"

s32 func_00293354(s32);

s16 func_00293324(void *);                          /* extern */
extern f32 D_800AA420;

void func_00292F48(void *arg0, s32 arg1, s32 arg2) {
    s16 temp_v0;

    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 0x10) = arg2;
    M2C_FIELD(arg0, s16 *, 0x1A) = 0;
    M2C_FIELD(arg0, s32 *, 0xC) = 0;
    M2C_FIELD(arg0, s32 *, 8) = arg1;
    if (func_00293354((s32) arg0) == 0x4D546864) {
        func_00293354((s32) arg0);
        if (((func_00293324(arg0) << 0x10) == 0) && (func_00293324(arg0) == 1)) {
            temp_v0 = func_00293324(arg0);
            M2C_FIELD(arg0, s16 *, 0x18) = temp_v0;
            if (!(temp_v0 & 0x8000)) {
                M2C_FIELD(arg0, f32 *, 0x14) = (f32) (D_800AA420 / (f32) temp_v0);
                if (func_00293354((s32) arg0) == 0x4D54726B) {
                    func_00293354((s32) arg0);
                    M2C_FIELD(arg0, s32 *, 4) = (s32) M2C_FIELD(arg0, s32 *, 8);
                }
            }
        }
    }
}
