#include "common.h"
#include "m2c_macros.h"

extern void func_002888BC(s32, s32, s32);
extern s32 func_004211AC(void);
extern s32 func_0042123C(void);
extern s32 func_0042AC0C(s32, s32, s32);
extern s32 D_800B6D64;
extern s32 D_800C2028;
extern u8 D_800F7078[];
extern s32 D_8011B118;
extern s32 D_8011F13C;

void func_00289110(s32 arg0) {
    D_800C2028 = -1;
    D_8011B118 = 0;
    func_002888BC(arg0, 0, 0);
    func_0042AC0C((s32)D_800F7078, 0, 0);
    func_004211AC();
    D_8011F13C = func_0042123C();
    D_800B6D64 = 1;
}
