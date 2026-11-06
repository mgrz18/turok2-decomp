#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"
#include "structs/boss.h"

s32 CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

void MotherBoss_OnIntroCinemaDone_2(void) {
    CAI__SetMode(D_8012FB70.obj, &D_8012FB70.obj->m_AI, 1);
}
