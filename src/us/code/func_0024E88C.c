#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


s32 CPlayer__SetState(CPlayer *player, CGameObjectInstance *actor, s32 state);

static __inline__ s32 func_0024E88C_isSpecial(CPlayer *player) {
    if (player->m_Mode == 18 || player->m_Mode == 16 || player->m_Mode == 17) {
        return 1;
    }
    return 0;
}

/* Puts the player into the water states: state 11 when the actor is
 * flagged 0x80 and the player is not already in a water state (8-11, 35;
 * in states 16-18 the player is flagged instead), state 9 when the actor
 * is just under the surface. Returns whether the state changed. */
s32 CPlayer__CheckWater(CPlayer *player, CGameObjectInstance *actor) {
    if (actor->m_AI.m_dwFlags & 0x80) {
        /* Nested ifs, not &&: GCC would fold 10 || 11 into a range test. */
        if (player->m_Mode != 10) if (player->m_Mode != 11) if (player->m_Mode != 8)
        if (player->m_Mode != 9) if (player->m_Mode != 35) {
            if (func_0024E88C_isSpecial(player)) {
                player->m_CharacterActor.m_AI.m_dwFlags |= 0x80;
                return 0;
            }
            CPlayer__SetState(player, actor, 11);
            return 1;
        }
    }
    if (func_0024E88C_isSpecial(player)) {
        return 0;
    }
    if ((actor->m_AI.m_dwFlags & 0xC0200) != 0x200) {
        return 0;
    }
    if (!(actor->m_vVelocity.y <= 0.0f)) {
        return 0;
    }
    if (-20.48f < actor->m_vVelocity.y) {
        if (player->m_Mode != 10) if (player->m_Mode != 11) if (player->m_Mode != 8)
        if (player->m_Mode != 9) if (player->m_Mode != 35) {
            CPlayer__SetState(player, actor, 9);
            return 1;
        }
    }
    return 0;
}
