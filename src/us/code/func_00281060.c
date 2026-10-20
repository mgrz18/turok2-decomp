#include "common.h"
#include "m2c_macros.h"
#include "structs/camera.h"

/* Starts a screen flash: the colour, the most opaque it gets, and how many
 * frames it takes to rise, hold and fade. */
void func_00281060(CCamera *cam, s8 maxOpacity, s8 red, s8 green, s32 blue, s32 attack, s32 sustain, s32 decay) {
    cam->m_MaxOpacity = maxOpacity;
    cam->m_FlashColor[0] = red;
    cam->m_FlashColor[1] = green;
    cam->m_FlashColor[3] = 0;
    cam->m_FlashTimer = 0.0f;
    cam->m_FlashMode = 1;
    cam->m_FlashColor[2] = blue;
    cam->m_AttackTime = attack;
    cam->m_SustainTime = sustain;
    cam->m_DecayTime = decay;
}
