#include "common.h"
#include "structs/game_object.h"

void func_00264DE0(CGameObjectInstance *obj, CAI *ai) {
    ai->m_ConditionTimer = 0.0f;
    obj->m_vVelocity.x = 0.0f;
    obj->m_vVelocity.z = 0.0f;
}
