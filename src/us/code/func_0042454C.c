#include "common.h"

s32 func_00266B80(void); /* osGetMemSize */
void func_00423E10(void);

extern s32 D_800C2038; /* current video mode */
extern s32 D_800C203C; /* applied video mode */
extern s32 D_800C2040;

void func_0042454C(void) {
    if (D_800C2038 == -1) {
        if (func_00266B80() != 0x400000) {
            D_800C2038 = 1;
        } else {
            D_800C2038 = 0;
        }
    }
    if (D_800C2040 < 5 && D_800C203C != D_800C2038) {
        func_00423E10();
        D_800C203C = D_800C2038;
    }
}
