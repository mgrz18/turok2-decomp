#include "common.h"
#ifdef TARGET_PORT
#include "structs/player.h"

/* Host body: the N64 code reads the intelligence at actor + 0x14 and its type and the dead
 * collision height at 0x00/0x34 (CEnemyIntelligence.m_CollisionDeadHeight). */
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
