#include "common.h"
#include "structs/game_object.h"

/* The definition value at 0x24, or 0x5334 when it is zero. */
s32 Destructible_GetIdleAnim(CGameObjectInstance *arg0) {
    s32 v = arg0->header.m_pIntelligence->m_Destructible.m_IdleAnim;
    if (v != 0) {
        return v;
    }
    return 0x5334;
}
