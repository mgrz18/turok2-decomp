#include "common.h"
#include "m2c_macros.h"

s32 Video_ApplyResolution(void);

extern s32 D_800C2038;

void Video_SetResolution(s32 arg0) {
    D_800C2038 = arg0;
    Video_ApplyResolution();
}
