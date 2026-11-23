#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Dart_AFF78(CGameObjectInstance *obj, sptr arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimDart_AFF78, obj->header.m_pIntelligence->m_Enemy.m_wDartAnimFlags);
}
