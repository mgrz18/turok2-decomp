#include "common.h"

void Video_ApplyResolution_2();                            /* extern */
extern s32 gResolution;

void Video_SetResolution_2(s32 arg0) {
    gResolution = arg0;
    Video_ApplyResolution_2();
}
