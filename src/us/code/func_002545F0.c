#include "common.h"
#include "structs/player.h"
#include "structs/region.h"

CRegionInfo *GameObject_GetRegionInfo(s32);                     /* extern */

void CPlayer__EaseEyeOffset(CPlayer *arg0, s32 arg1) {
    f32 temp_f1;
    CRegionInfo *temp_v0;

    temp_v0 = GameObject_GetRegionInfo(arg1);
    if (temp_v0 != NULL) {
        arg0->m_ActualCameraVertEyeOffset += (temp_v0->unk_2C - arg0->m_ActualCameraVertEyeOffset) * 0.2f;
    }
}
