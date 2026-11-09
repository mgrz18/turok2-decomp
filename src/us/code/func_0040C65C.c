#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "protos/ai.h"


void MotherBoss_OnIntroCinemaDone(void) {
    CAI__SetMode(gMotherBoss.obj, &gMotherBoss.obj->m_AI, 1);
}
