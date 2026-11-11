#include "common.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"

/* The port: the definition by member (N64: m_pIntelligence at 0x14, a pointer at a different host
 * offset; the player's death crashed here). The definition is asset data and keeps its N64 byte
 * offsets: 0x34 is m_Enemy.m_CollisionDeadHeight (the player's and the turret's too). */
f32 GameObject_GetDeadCollisionHeight(u8 *actor) {
    u8 *def = (u8 *) ((CGameObjectInstance *) actor)->header.m_pIntelligence;

    switch (*(s32 *) def) {
        case 1:
        case 4:
        case 11:
            return *(f32 *) (def + 0x34);
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
