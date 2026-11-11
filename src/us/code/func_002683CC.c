#include "common.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/intelligence.h"

/* host body (round 8 w2): the N64 view below reads the definition at the N64 0x14 of the
 * instance; the first enemy killed on the host died here. */
f32 GameObject_GetDeadCollisionHeight(CGameObjectInstance *actor) {
    CIntelligence *def = actor->header.m_pIntelligence;

    switch (def->m_Type) {
        case 1:
        case 4:
        case 11:
            return def->m_Enemy.m_CollisionDeadHeight;
    }
    return 0.0f;
}
#else

f32 GameObject_GetDeadCollisionHeight(u8 *actor) {
    u8 *def = *(u8 **) (actor + 0x14);

    switch (*(s32 *) def) {
        case 1:
        case 4:
        case 11:
            return *(f32 *) (def + 0x34);
    }
    return 0.0f;
}
#endif
