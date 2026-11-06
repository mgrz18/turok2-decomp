#include "common.h"
#include "structs/camera.h"
#include "globals/camera.h"

/* Switches the camera to mode arg1 and runs its enter handler. */
void CCamera__SetMode(CCamera *arg0, s16 arg1) {
    void (*temp_v0)(void);

    arg0->m_Mode = arg1;
    arg0->m_Time = 0.0f;
    arg0->m_Stage = 0;
    arg0->m_StageTime = 0.0f;
    temp_v0 = gCameraModeTable[arg0->m_Mode].enter;
    if (temp_v0 != NULL) {
        temp_v0();
    }
}
