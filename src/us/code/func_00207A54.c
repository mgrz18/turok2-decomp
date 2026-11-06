#include "common.h"
#include "m2c_macros.h"

extern s32 sLoadedTexture;

void Texture_InvalidateCache(void) {
    sLoadedTexture = 0;
}
