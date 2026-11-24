#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/enemy_ai.h"
#include "structs/game_object.h"


void EnemyAI_SleepIfOffscreen(CGameObjectInstance *arg0, CAI *arg1, s32 arg2) {
    if ((CScene__GetInstanceSavedBit(&gScene, arg0) == 0) && (arg1->m_Mode == 0)) {
        EnemyAI_CheckOffscreen(arg0, arg1, arg2);
        CAI__SetMode(arg0, arg1, 2);
    }
}
