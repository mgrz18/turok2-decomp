#include "common.h"

f32 GameObject_GetGroundHeight(void *actor, s32 arg);

/* Whether the actor stands on (or within 0.512 above) the ground. */
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port: by member of CGameObjectInstance (N64: the flags 0xD4, the AI's flags 0x140, the
 * position's y 0x8). */
s32 GameObject_IsOnGround(u8 *actor_) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;
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
    return actor->header.m_vPos.y - GameObject_GetGroundHeight(actor, 0) <= 0.512f;
}
#else
s32 GameObject_IsOnGround(u8 *actor) {
    s32 state;

    if (actor[0] == 1) {
        if (*(s32 *) (actor + 0xD4) & 0x1000) {
            return 1;
        }
        state = *(s32 *) (actor + 0x140);
        if ((state & 0xC0000) && !(state & 0x40000)) {
            return 1;
        }
    }
    return *(f32 *) (actor + 0x8) - GameObject_GetGroundHeight(actor, 0) <= 0.512f;
}
#endif
