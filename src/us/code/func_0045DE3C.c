#include "common.h"
#include "m2c_macros.h"
#include "globals/menu.h"
#include "globals/scene.h"

s32 Level_GetHub(s32);


void MissionMenu_InitStatus_2(void) {
    sMissionHub = Level_GetHub(gScene_m_LevelID);
    sMissionFailed = 0;
}
