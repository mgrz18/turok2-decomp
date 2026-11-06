/* Second VM module copy of CPlayer__SetupActors (same source, linked 0x0042D980). */
#include "common.h"
#include "structs/game.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/player.h"
#include "globals/game.h"

extern void CGameObjectInstance__ConstructFromRecord(void *obj, void *rec, s32 mode, s32 index);
extern void CGameObjectInstance__CalculateOrientationMatrix();
extern void Actor_PlayRangedSound();
extern void Player_OnMultiplayerKill();
extern void CPlayer__OnParticleHitCallback();
extern void Player_DrawNodeCallback();
extern void WeaponActor_BuildMatrix();
extern void CAI__SetMode(void *, void *, s32);
extern void WeaponActor_SwitchToRequested(void *, void *);
extern void CPlayer__UpdateOrbitCamera(CPlayer *p, CPlayer *q);
extern void Cinema_UpdateCameraIfActive(s32);

void CPlayer__SetupActors_2(CPlayer *p, void *rec, s32 mode) {
    f32 half;

    p->m_CharacterActor.m_AI.m_pParent = p;
    CGameObjectInstance__ConstructFromRecord(p, rec, mode, 0);
    p->m_CharacterActor.m_AI.m_pParent = p;
    p->m_CharacterActor.m_dwFlags |= 0x100000;
    p->m_CharacterActor.m_dwFlags |= 0x10000;
    p->m_CharacterActor.m_dwFlags |= 0x1000000;
    p->m_CharacterActor.m_dwFlags |= 0x2000000;
    if (!gEngineApp_m_Settings.multiplayer) {
        p->m_CharacterActor.m_dwFlags |= 0x10000000;
    }
    p->m_CharacterActor.m_AI.m_pfCalculateOrientationMatrix = CGameObjectInstance__CalculateOrientationMatrix;
    p->m_CharacterActor.m_AI.m_pfEffects = Actor_PlayRangedSound;
    p->m_CharacterActor.m_AI.m_pfHurt = Player_OnMultiplayerKill;
    p->m_CharacterActor.m_AI.m_pfHit = CPlayer__OnParticleHitCallback;
    if (gEngineApp_m_Settings.multiplayer) {
        p->m_CharacterActor.m_AI.m_pfDraw = Player_DrawNodeCallback;
    }

    p->m_WeaponActor.m_AI.m_pParent = p;
    CGameObjectInstance__ConstructFromRecord(&p->m_WeaponActor, rec, mode, 0);
    p->m_WeaponActor.m_AI.m_pfCalculateOrientationMatrix = WeaponActor_BuildMatrix;
    p->m_WeaponActor.m_AI.m_Mode = -1;
    p->m_WeaponActor.m_AI.m_pParent = p;
    p->m_WeaponActor.m_AI.m_pfHurt = Player_OnMultiplayerKill;
    p->m_WeaponActor.m_AI.m_pfHit = CPlayer__OnParticleHitCallback;
    p->m_WeaponActor.m_dwFlags |= 0x200000;
    p->m_Data.m_CurrentWeapon = -1;
    half = 0.5f;
    p->m_WeaponActor.m_vScale.x = half;
    p->m_WeaponActor.m_vScale.y = half;
    p->m_WeaponActor.m_vScale.z = half;
    p->m_WeaponActor.m_dwFlags &= ~0x10000;
    CAI__SetMode(&p->m_WeaponActor, &p->m_WeaponActor.m_AI, 0);
    WeaponActor_SwitchToRequested(&p->m_WeaponActor, &p->m_WeaponActor.m_AI);
    CPlayer__UpdateOrbitCamera(p, p);
    Cinema_UpdateCameraIfActive(p->m_pCamera);
    p->m_CharacterActor.m_vScale.x = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_ScaleX;
    p->m_CharacterActor.m_vScale.y = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_ScaleY;
    p->m_CharacterActor.m_vScale.z = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_ScaleZ;
}
