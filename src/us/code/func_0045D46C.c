/* Second-module copy of func_0042454C (first module; this one at link address 0x0042446C), calling this module's own functions at their link addresses. */
#include "common.h"

s32 func_00266B80(void); /* osGetMemSize */
void func_00423D30(void);

extern s32 D_800C2038; /* current video mode */
extern s32 D_800C203C; /* applied video mode */
extern s32 D_800C2040;

void func_0045D46C(void) {
    if (D_800C2038 == -1) {
        if (func_00266B80() != 0x400000) {
            D_800C2038 = 1;
        } else {
            D_800C2038 = 0;
        }
    }
    if (D_800C2040 < 5 && D_800C203C != D_800C2038) {
        func_00423D30();
        D_800C203C = D_800C2038;
    }
}
