#include "common.h"
#include "m2c_macros.h"

s32 Video_ApplyResolution(void);

extern s32 gResolution;

void Video_SetResolution(s32 arg0) {
    gResolution = arg0;
    Video_ApplyResolution();
}
