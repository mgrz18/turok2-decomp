#include "common.h"
#include "m2c_macros.h"
#include "structs/player.h"
#include "globals/engine.h"


void CPlayer__TickFlashlight(CPlayer *arg0) {
    f32 temp_f0;
    f32 temp_f1;

    temp_f1 = arg0->m_FlashlightTime;
    if (temp_f1 > 0.0f) {
        temp_f0 = temp_f1 - gFrameIncrement;
        arg0->m_FlashlightTime = temp_f0;
        if (temp_f0 < 0.0f) {
            arg0->m_FlashlightTime = 0.0f;
        }
    }
}
