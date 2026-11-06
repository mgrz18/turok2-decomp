#include "common.h"
#include "structs/player.h"

extern u8 gScene[];
extern CIntelligence *CScene__FindIntelligence(void *spawns, s32 kind, s32 character);
extern s32 Random_Next(void);

/* Same as CPlayer__PickSpawnPoint. */
static __inline__ void place_at_spawn(CPlayer *p) {
    CIntelligence *sp;
    s32 character = p->m_Data.m_nDeathmatchCharacter;

    if (p->m_pOptions->m_TagMode == 1) {
        character = 11;
    }
    sp = CScene__FindIntelligence(gScene, 11, character);
    if (sp == NULL) {
        sp = CScene__FindIntelligence(gScene, 11, -1);
        if (sp == NULL) {
            sp = CScene__FindIntelligence(gScene, -1, -1);
        }
    }
    p->m_CharacterActor.header.m_pIntelligence = sp;
    p->m_CharacterActor.m_vScale.x = sp->m_Player.m_ScaleX;
    p->m_CharacterActor.m_vScale.y = sp->m_Player.m_ScaleY;
    p->m_CharacterActor.m_vScale.z = sp->m_Player.m_ScaleZ;
}

/* Respawn a player at a spawn point with full health. */
void CPlayer__Respawn(CPlayer *p) {
    place_at_spawn(p);
    p->m_Data.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
    p->m_CharacterActor.m_AI.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
    p->m_CharacterActor.m_AI.m_nTexture = (p->m_Data.m_nDeathmatchCharacter != 7) ? -1 : Random_Next() % 18;
}
