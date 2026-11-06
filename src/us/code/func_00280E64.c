#include "common.h"
#include "structs/camera.h"

/* An entry of the camera mode table at D_800B6CB0: the handler run on entering the mode. */
typedef struct {
    /* 0x00 */ s32 (*enter)();
    /* 0x04 */ s32 unk4;
} CameraModeEntry;

extern CameraModeEntry D_800B6CB0[];

/* Switches the camera to mode arg1 and runs its enter handler. */
void CCamera__SetMode(CCamera *arg0, s16 arg1) {
    s32 (*temp_v0)();

    arg0->m_Mode = arg1;
    arg0->m_Time = 0.0f;
    arg0->m_Stage = 0;
    arg0->m_StageTime = 0.0f;
    temp_v0 = D_800B6CB0[arg0->m_Mode].enter;
    if (temp_v0 != NULL) {
        temp_v0();
    }
}
