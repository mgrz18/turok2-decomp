#include "common.h"
#include "structs/game_object.h"

s32 func_002627EC(CGameObjectInstance *arg0) {
    s32 v = arg0->header.m_pIntelligence->m_Enemy.m_StartAnim;

    if (v != 0) {
        return v;
    }
    return 0x2F44;
}
