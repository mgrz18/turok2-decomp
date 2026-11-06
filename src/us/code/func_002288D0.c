#include "common.h"
#include "m2c_macros.h"
#include "globals/render.h"
#include "globals/scene.h"

void DLights_SwapBuffers(void) {
    gDLightBufferIndex = gDLightBufferOffset;
    if (gDLightBufferOffset == 0) {
        gDLightBufferOffset = 0x20;
        return;
    }
    gDLightBufferOffset = 0;
}
