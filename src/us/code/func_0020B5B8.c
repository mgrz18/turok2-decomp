#include "common.h"
#include "m2c_macros.h"

extern s32 D_800AB880;
extern s32 D_800AB884;

void Render_InvalidateModeCache(void) {
    D_800AB884 = -1;
    D_800AB880 = -1;
}
