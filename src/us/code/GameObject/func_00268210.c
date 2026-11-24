/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024E1F8.c, func_0024E1F8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"

s32 Cinema_PathPlaying(void);

/* Enemies and throwables always; a player's body only while its flags 0x300000 are set and its
 * owner (the CPlayer) rides an enemy or a cinema path plays. Round 28 joined the N64 view and the
 * port's (round 11 C26) host-offset view into one on CGameObjectInstance: the owner is a player,
 * its 0xBB0 m_pOnEnemy. */
s32 func_00268210(CGameObjectInstance *obj) {
    switch (obj->header.m_pIntelligence->m_Type) {
    case INTELLIGENCE_PLAYER:
        if (obj->header.m_Type == 1 && (obj->m_dwFlags & 0x300000)
            && (((CPlayer *) obj->m_AI.m_pParent)->m_pOnEnemy != NULL || Cinema_PathPlaying() != 0)) {
            return 1;
        }
        return 0;
    case INTELLIGENCE_ENEMY:
    case INTELLIGENCE_THROWABLE:
        return 1;
    default:
        return 0;
    }
}
