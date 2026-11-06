#include "common.h"
#include "m2c_macros.h"

extern s32 D_800AB570;

void Texture_InvalidateCache(void) {
    D_800AB570 = 0;
}
