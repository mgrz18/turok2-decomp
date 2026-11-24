#include "common.h"
#include "protos/game_object.h"

/* Whether the actor stands on (or within 0.512 above) the ground. The ROM passes a second
 * argument (0) that GameObject_GetGroundHeight ignores. */
s32 GameObject_IsOnGround(CGameObjectInstance *actor) {
    s32 state;

    if (actor->header.m_Type == 1) {
        if (actor->m_dwFlags & 0x1000) {
            return 1;
        }
        state = actor->m_AI.m_dwFlags;
        if ((state & 0xC0000) && !(state & 0x40000)) {
            return 1;
        }
    }
    return actor->header.m_vPos.y - ((f32 (*)(CGameObjectInstance *, s32)) GameObject_GetGroundHeight)(actor, 0) <= 0.512f;
}
