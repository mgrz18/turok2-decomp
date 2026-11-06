#include "common.h"
#include "structs/camera.h"
#include "globals/game.h"


/* The camera's fog colour, each channel to 5 bits, or black while flag
 * 0x4000 is up. */
void CCamera__GetFogColor(CCamera *cam, u8 *red, u8 *green, u8 *blue) {
    if (gEngineApp_m_Settings.cheatFlags & 0x4000) {
        *red = 0;
        *green = 0;
        *blue = 0;
        return;
    }
    *red = cam->m_FogColor[0] & 0xF8;
    *green = cam->m_FogColor[1] & 0xF8;
    *blue = cam->m_FogColor[2] & 0xF8;
}
