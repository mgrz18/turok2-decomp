#include "common.h"

s32 func_00204EDC(s32, s32, s32, s32);

s32 func_00266C5C(s32, s32, s32);       /* extern */
extern s32 D_800C18A0;
extern s32 D_800C2220;
extern s32 D_800C2224;
extern s32 *D_800C2228;

void func_0045F2FC(void) {
    s32 *temp_v0;
    s32 temp_a0;

    temp_v0 = func_00204EDC(0, 0x810, 0x23, (s32) &D_800C18A0);
    temp_a0 = *temp_v0;
    D_800C2228 = temp_v0;
    D_800C2224 = temp_a0;
    func_00266C5C(temp_a0, 0, 0x810);
    D_800C2220 = 1;
}
