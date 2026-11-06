#include "common.h"
#include "structs/game_object.h"

s32 Destructible_GetGoAnim(CGameObjectInstance *arg0) {
    return arg0->header.m_pIntelligence->m_Destructible.m_GoAnim;
}
