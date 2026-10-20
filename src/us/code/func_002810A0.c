#include "common.h"
#include "structs/camera.h"

extern s32 D_8011AAD8;          /* game flags */

/* The camera's fog colour, each channel to 5 bits, or black while flag
 * 0x4000 is up. */
void func_002810A0(CCamera *cam, u8 *red, u8 *green, u8 *blue) {
    if (D_8011AAD8 & 0x4000) {
        *red = 0;
        *green = 0;
        *blue = 0;
        return;
    }
    *red = cam->m_FogColor[0] & 0xF8;
    *green = cam->m_FogColor[1] & 0xF8;
    *blue = cam->m_FogColor[2] & 0xF8;
}
