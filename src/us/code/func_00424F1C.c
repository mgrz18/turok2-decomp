#include "common.h"
#include "m2c_macros.h"
#include "globals/menu.h"

s32 Level_GetHub(s32);

extern s32 gScene_m_LevelID;

void MissionMenu_InitStatus(void) {
    sMissionHub = Level_GetHub(gScene_m_LevelID);
    sMissionFailed = 0;
}
