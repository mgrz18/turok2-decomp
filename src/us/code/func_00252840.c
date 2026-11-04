#include "common.h"
#include "structs/player.h"

extern u8 D_800F7078[];
extern CIntelligence *func_00225EBC(void *spawns, s32 kind, s32 character);
extern s32 func_002113A4(void);

/* Same as func_00254044. */
static __inline__ void place_at_spawn(CPlayer *p) {
    CIntelligence *sp;
    s32 character = p->m_Data.m_nDeathmatchCharacter;

    if (p->m_pOptions->m_TagMode == 1) {
        character = 11;
    }
    sp = func_00225EBC(D_800F7078, 11, character);
    if (sp == NULL) {
        sp = func_00225EBC(D_800F7078, 11, -1);
        if (sp == NULL) {
            sp = func_00225EBC(D_800F7078, -1, -1);
        }
    }
    p->m_CharacterActor.header.m_pIntelligence = sp;
    p->m_CharacterActor.m_vScale.x = sp->m_Player.m_ScaleX;
    p->m_CharacterActor.m_vScale.y = sp->m_Player.m_ScaleY;
    p->m_CharacterActor.m_vScale.z = sp->m_Player.m_ScaleZ;
}

/* Respawn every player at a spawn point, with full health, out of tag mode.
 * A goto loop: as a for/do loop GCC hoists the constant 1 out of it. */
void func_00252840(CPlayerList *list) {
    CPlayer *p;

    p = list->m_ActiveList.m_pHead;
    if (p == NULL) return;
loop:
    {
        p->m_pOptions->m_TagMode = 0;
        place_at_spawn(p);
        p->m_Data.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
        p->m_CharacterActor.m_AI.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
        p->m_CharacterActor.m_AI.m_nTexture = (p->m_Data.m_nDeathmatchCharacter != 7) ? -1 : func_002113A4() % 18;
        p = p->m_pNext;
        if (p != NULL) goto loop;
    }
}
