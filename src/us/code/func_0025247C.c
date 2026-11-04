#include "common.h"
#include "structs/player.h"

extern u8 D_800F7078[];
extern CIntelligence *func_00225EBC(void *spawns, s32 kind, s32 character);
extern s32 func_002113A4(void);
extern void func_00275624(s32 sound);
extern void func_00243414(void *weapon, void *weaponAI, s32 arg2);

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

/* Put the player into tag mode ("it"): a sound, the weapon wheel and the
 * weapon reset, then respawn with full health. */
void func_0025247C(CPlayer *p) {
    CBarrel *barrel;

    p->m_pOptions->m_TagMode = 1;
    func_00275624(0x27D8);
    barrel = &p->m_Barrel;
    barrel->m_State = 0;
    barrel->m_Turn = 0;
    barrel->m_nCursor = -1;
    barrel->m_Open = 0;
    p->m_bHeadMovementDisabled = 0;
    func_00243414((u8 *)p + 0x288, (u8 *)p + 0x3C8, 1);
    place_at_spawn(p);
    p->m_Data.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
    p->m_CharacterActor.m_AI.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
    p->m_CharacterActor.m_AI.m_nTexture = (p->m_Data.m_nDeathmatchCharacter != 7) ? -1 : func_002113A4() % 18;
}
