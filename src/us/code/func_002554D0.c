#include "common.h"
#include "structs/game.h"
#include "structs/game_object.h"
#include "structs/player.h"


/* gCollisionResults is the collision results (CCollisionResults, structs/collision.h);
 * this reads its first word, m_pInstance (what the last test hit), as a
 * pointer variable: gCollisionResults.m_pInstance does not give this code. */
extern CGameObjectInstance *gCollisionResults; /* local type: the code reads m_pInstance through this pointer view (see above) */

void CPlayer__Respawn(CPlayer *p);

static __inline__ u8 ActorKind(CGameObjectInstance *actor) {
    return actor->header.m_Type;
}

/* In multiplayer, when actor 0x64E is a kind-1 actor whose 0x174/0x176 bytes
 * differ, ends the player's tag mode 1 and records it. */
s32 CPlayer__CheckTagHit(CPlayer *p) {
    CGameObjectInstance **slot;
    CGameObjectInstance *a;
    CPlayerOptions *opts;
    s32 kind;
    s32 mode;

    if (gCollisionResults != NULL) {
        slot = &gCollisionResults; /* read through the slot: the ROM copies the pointer */
        a = *slot;
        kind = a->header.m_Type;
        if (kind == 1 && gEngineApp_m_Game.m_Settings.multiplayer && gCollisionResults->m_nTypeFlag == 0x64E
            && gCollisionResults->m_AI.m_ModeBefore != gCollisionResults->m_AI.m_Mode) {
            opts = p->m_pOptions;
            mode = opts->m_TagMode;
            if (mode == kind) {
                opts->m_TagMode = 0;
                p->m_pOptions->m_LastTagMode = mode;
                CPlayer__Respawn(p);
                gEngineApp_m_Game.m_Settings.match.tagPhase = mode;
            }
        }
    }
    return 0;
}
