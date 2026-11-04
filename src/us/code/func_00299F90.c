#include "common.h"

void func_0029B030(void *, void *, s32); /* extern */
void osSendMesg(void *, s32, s32); /* extern */
extern s32 D_800B7A50;
extern u8 D_80121670[];
extern u8 D_80121678[];

void func_00299F90(void) {
    D_800B7A50 = 1;
    func_0029B030(D_80121678, D_80121670, 1);
    osSendMesg(D_80121678, 0, 0);
}
