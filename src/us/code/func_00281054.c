#include "common.h"
#include "m2c_macros.h"
#include "structs/camera.h"

/* Sets the flash level and how fast it fades. */
void CCamera__SetHitFlash(CCamera *cam, s8 flash, s8 dec) {
    cam->m_Flash = flash;
    cam->m_FlashDec = dec;
}
