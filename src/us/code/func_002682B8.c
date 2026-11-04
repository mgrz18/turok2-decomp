#include "common.h"
#include "structs/game_object.h"

s32 func_002682B8(CGameObjectInstance *obj) {
    return obj->header.m_pIntelligence->m_Type == 1;
}
