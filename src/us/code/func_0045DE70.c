#include "common.h"
#include "m2c_macros.h"

s32 Level_GetHub(s32);

extern s32 gScene_m_LevelID;
extern s32 sMissionHub;
extern s32 sMissionFailed;

void MissionMenu_InitFailed_2(void) {
    sMissionHub = Level_GetHub(gScene_m_LevelID);
    sMissionFailed = 1;
}
