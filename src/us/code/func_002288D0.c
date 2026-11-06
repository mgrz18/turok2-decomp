#include "common.h"
#include "m2c_macros.h"
#include "globals/scene.h"

extern s32 gDLightBufferIndex;

void DLights_SwapBuffers(void) {
    gDLightBufferIndex = gDLightBufferOffset;
    if (gDLightBufferOffset == 0) {
        gDLightBufferOffset = 0x20;
        return;
    }
    gDLightBufferOffset = 0;
}
