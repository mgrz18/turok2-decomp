#include "common.h"
#include "structs/game_object.h"

s32 GameObject_TiltsToGround(CGameObjectInstance *obj) {
    return ((u32) obj->header.m_pIntelligence->m_dwFlags >> 2) & 1;
}
