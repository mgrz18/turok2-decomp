#include "common.h"
#include "structs/player.h"

/* Spawn point record from func_00225EBC: a position and a starting health. */
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 health;
    /* 0x1C */ u8 pad1C[0x38 - 0x1C];
    /* 0x38 */ f32 pos[3];
} SpawnPoint;

extern u8 D_800F7078[];
extern SpawnPoint *func_00225EBC(void *spawns, s32 kind, s32 character);
extern s32 func_002113A4(void);

/* Same as func_00254044. */
static __inline__ void place_at_spawn(CPlayer *p) {
    SpawnPoint *sp;
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
    p->m_CharacterActor.m_vScale.x = sp->pos[0];
    p->m_CharacterActor.m_vScale.y = sp->pos[1];
    p->m_CharacterActor.m_vScale.z = sp->pos[2];
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
        p->m_Data.m_Health = ((SpawnPoint *)p->m_CharacterActor.header.m_pIntelligence)->health << 8;
        p->m_CharacterActor.m_AI.m_Health = ((SpawnPoint *)p->m_CharacterActor.header.m_pIntelligence)->health << 8;
        p->m_CharacterActor.m_AI.m_nTexture = (p->m_Data.m_nDeathmatchCharacter != 7) ? -1 : func_002113A4() % 18;
        p = p->m_pNext;
        if (p != NULL) goto loop;
    }
}
