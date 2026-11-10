#include "common.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/player.h"

/* The port: the collision radius by member (N64: the definition at 0x14, the flags at 0xD4, the
 * AI's m_pParent at 0x1A8 = the player, whose m_pOnEnemy is at 0xBB0). The intelligence is asset
 * data: its radius words stay byte offsets (0x2C enemy/turret/player, 0x18 throwable). */
f32 func_00268460(u8 *actor_) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;
    u8 *def = (u8 *) actor->header.m_pIntelligence;
    CGameObjectInstance *weapon;

    switch (*(s32 *) def) {
        case 11:
            if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
                weapon = ((CPlayer *) actor->m_AI.m_pParent)->m_pOnEnemy;
                if (weapon != NULL) {
                    return func_00268460((u8 *) weapon);
                }
            }
            return *(f32 *) (def + 0x2C);
        case 1:
        case 4:
            return *(f32 *) (def + 0x2C);
        case 5:
            return *(f32 *) (def + 0x18);
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
