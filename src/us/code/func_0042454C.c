#include "common.h"

s32 GetMemSize(void); /* osGetMemSize */
void Video_ApplyResolution(void);

extern s32 D_800C2038; /* current video mode */
extern s32 D_800C203C; /* applied video mode */
extern s32 D_800C2040;

void Video_UpdateResolution(void) {
    if (D_800C2038 == -1) {
        if (GetMemSize() != 0x400000) {
            D_800C2038 = 1;
        } else {
            D_800C2038 = 0;
        }
    }
    if (D_800C2040 < 5 && D_800C203C != D_800C2038) {
        Video_ApplyResolution();
        D_800C203C = D_800C2038;
    }
}
