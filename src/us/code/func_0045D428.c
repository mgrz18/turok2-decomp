#include "common.h"

void Video_ApplyResolution_2();                            /* extern */
extern s32 D_800C2038;

void Video_SetResolution_2(s32 arg0) {
    D_800C2038 = arg0;
    Video_ApplyResolution_2();
}
