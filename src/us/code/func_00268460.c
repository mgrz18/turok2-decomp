#include "common.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/player.h"

/* The port: the same on host types (the N64 text reads the instance and the player at N64
 * offsets: 0x14 the intelligence, 0xD4 the flags, 0x1A8 the AI parent, CPlayer 0xBB0 the
 * mounted enemy). The wall radius of a collision sweep: the enemy/turret/player definition's
 * m_CollisionWallRadius (0x2C), the throwable's radius (0x18); a riding player uses its mount's. */
f32 func_00268460(u8 *actor) {
    CGameObjectInstance *obj = (CGameObjectInstance *) actor;
    CIntelligence *def = obj->header.m_pIntelligence;
    CGameObjectInstance *mount;

    switch (def->m_Type) {
        case 11:
            if (obj->header.m_Type == 1 && (obj->m_dwFlags & 0x300000)) {
                mount = ((CPlayer *) obj->m_AI.m_pParent)->m_pOnEnemy;
                if (mount != NULL) {
                    return func_00268460((u8 *) mount);
                }
            }
            return *(f32 *) ((u8 *) def + 0x2C);
        case 1:
        case 4:
            return *(f32 *) ((u8 *) def + 0x2C);
        case 5:
            return *(f32 *) ((u8 *) def + 0x18);
    }
    return 0.0f;
}
#else

f32 func_00268460(u8 *actor) {
    u8 *def = *(u8 **) (actor + 0x14);
    u8 *weapon;

    switch (*(s32 *) def) {
        case 11:
            if (actor[0] == 1 && (*(s32 *) (actor + 0xD4) & 0x300000)) {
                weapon = *(u8 **) (*(u8 **) (actor + 0x1A8) + 0xBB0);
                if (weapon != NULL) {
                    return func_00268460(weapon);
                }
            }
            return *(f32 *) (*(u8 **) (actor + 0x14) + 0x2C);
        case 1:
        case 4:
            return *(f32 *) (def + 0x2C);
        case 5:
            return *(f32 *) (def + 0x18);
    }
    return 0.0f;
}
#endif
