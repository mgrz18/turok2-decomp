#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

s32 CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

void MotherBoss_OnPhase2CinemaDone(void) {
    CAI__SetMode(gMotherBoss.obj, &gMotherBoss.obj->m_AI, 0x14);
}
