#include "common.h"
#include "structs/game_object.h"

s32 GameObject_IsEnemy(CGameObjectInstance *obj) {
    return obj->header.m_pIntelligence->m_Type == 1;
}
