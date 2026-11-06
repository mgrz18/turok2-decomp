#include "common.h"
#include "m2c_macros.h"

s32 Level_GetHub(s32);

extern s32 D_80110048;
extern s32 D_80130A30;
extern s32 D_80130A34;

void MissionMenu_InitStatus_2(void) {
    D_80130A30 = Level_GetHub(D_80110048);
    D_80130A34 = 0;
}
