/* Second-module copy of Video_UpdateResolution (first module; this one at link address 0x0042446C), calling this module's own functions at their link addresses. */
#include "common.h"

s32 GetMemSize(void); /* osGetMemSize */
void Video_ApplyResolution_2(void);

extern s32 D_800C2038; /* current video mode */
extern s32 D_800C203C; /* applied video mode */
extern s32 D_800C2040;

void Video_UpdateResolution_2(void) {
    if (D_800C2038 == -1) {
        if (GetMemSize() != 0x400000) {
            D_800C2038 = 1;
        } else {
            D_800C2038 = 0;
        }
    }
    if (D_800C2040 < 5 && D_800C203C != D_800C2038) {
        Video_ApplyResolution_2();
        D_800C203C = D_800C2038;
    }
}
