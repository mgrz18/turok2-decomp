#include "common.h"
#include "structs/camera.h"

extern f32 D_800B6D28;          /* the frame's time step */

/* Runs the screen flash (func_00281060 starts it): rises over m_AttackTime
 * frames to m_MaxOpacity, holds for m_SustainTime, fades over m_DecayTime,
 * the opacity in m_FlashColor[3]. */
void func_0027C074(CCamera *cam) {
    switch ((u32) cam->m_FlashMode) {     /* unsigned: "not 1, below 2" is then just 0 */
        case 0:                 /* none */
            break;
        case 1:                 /* rising */
            if (cam->m_AttackTime <= cam->m_FlashTimer) {
                cam->m_FlashMode = 2;
                cam->m_FlashTimer = 0.0f;
                goto hold;
            }
            cam->m_FlashColor[3] = (u32) (cam->m_FlashTimer / (cam->m_AttackTime + 1) * cam->m_MaxOpacity);
            break;
        case 2:                 /* holding */
        hold:
            if (!(cam->m_SustainTime <= cam->m_FlashTimer)) {
                cam->m_FlashColor[3] = cam->m_MaxOpacity;
                break;
            }
            cam->m_FlashMode = 3;
            cam->m_FlashTimer = 0.0f;
        case 3:                 /* fading */
            if (cam->m_DecayTime <= cam->m_FlashTimer) {
                cam->m_FlashMode = 0;
                cam->m_FlashTimer = 0.0f;
                break;
            }
            cam->m_FlashColor[3] = (u32) (cam->m_MaxOpacity - cam->m_FlashTimer / (cam->m_DecayTime + 1) * cam->m_MaxOpacity);
            break;
    }
    cam->m_FlashTimer += D_800B6D28;
}
