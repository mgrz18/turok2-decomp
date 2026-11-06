#include "common.h"
#include "m2c_macros.h"
#include "globals/render.h"


void Texture_InvalidateCache(void) {
    sLoadedTexture = 0;
}
