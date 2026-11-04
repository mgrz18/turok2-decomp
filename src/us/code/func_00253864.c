#include "common.h"
#include "structs/player.h"

extern void func_0021F1B4(CGameObjectInstance *);
extern s32 func_0021A344(CGameObjectInstance *);
extern f32 D_800B6D28;

/* Rebuilds the weapon actor (keeping D_800B6D28 across it) and puts it where
 * the character is. */
void func_00253864(CPlayer *player) {
    CGameObjectInstance *weapon = &player->m_WeaponActor;
    s32 oldAnim = player->m_CharAnim;
    f32 saved = D_800B6D28;
    s32 changed;

    weapon->m_dwHotPointFlags = 0x10;
    weapon->m_dwFlags &= ~0x20000;
    func_0021F1B4(weapon);
    func_0021A344(weapon);
    changed = oldAnim != player->m_CharAnim;
    D_800B6D28 = saved;
    if (changed) {
        player->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted = 0;
    }
    player->m_WeaponActor.header.m_vPos = player->m_CharacterActor.header.m_vPos;
    player->m_WeaponActor.m_RotY = player->m_CharacterActor.m_RotY;
    player->m_WeaponActor.header.m_pCurrentRegion = player->m_CharacterActor.header.m_pCurrentRegion;
    player->m_WeaponActor.m_qGround = player->m_CharacterActor.m_qGround;
}
