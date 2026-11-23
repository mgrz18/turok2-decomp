#include "common.h"
#include "globals/render.h"

void Video_ApplyResolution_2();                            /* extern */

void Video_SetResolution_2(s32 arg0) {
    gResolution = arg0;
    Video_ApplyResolution_2();
}
