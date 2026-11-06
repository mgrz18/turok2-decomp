#include "common.h"
#include "globals/render.h"

void Render_InvalidateModeCache(void);

void func_0020B554(void) {
    Render_InvalidateModeCache();
    if (gFogEnabled) {
        gFogGeometryMode = 0x10000;
        gFogRenderModeFogA = 0xC4000000;
        gFogRenderModeShadeA = 0xC8000000;
    } else {
        gFogGeometryMode = 0;
        gFogRenderModeFogA = 0x0C080000;
        gFogRenderModeShadeA = 0x0C080000;
    }
}
