#include "common.h"
#include "m2c_macros.h"
#include "globals/render.h"

s32 Video_ApplyResolution(void);


void Video_SetResolution(s32 arg0) {
    gResolution = arg0;
    Video_ApplyResolution();
}
