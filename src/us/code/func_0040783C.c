#include "common.h"
#include "structs/game_object.h"

void func_0040783C(CGameObjectInstance *obj) {
    obj->m_vVelocity.x = 0.0f;
    obj->m_vVelocity.y = 0.0f;
    obj->m_vVelocity.z = 0.0f;
    obj->m_dwFlags = (s32) (obj->m_dwFlags | 0x2000);
}
