/* CPlayer code: func_0042DA60, 0x0042DA60-0x0042DC18 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/game.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/player.h"

extern f32 D_800A6A98; /* 0.5f, in the engine's .rodata */

extern void func_0021E6AC(void *obj, void *rec, s32 mode, s32 index);
extern void func_0021E840();
extern void func_00256738();
extern void func_00247C10();
extern void func_0025689C();
extern void func_00247FB0();
extern void func_00257A40();
extern void func_00243414(void *, void *, s32);
extern void func_00258668(void *, void *);
extern void func_0024CF58(CPlayer *p, CPlayer *q);
extern void func_00284254(s32);

void func_0042DA60(CPlayer *p, void *rec, s32 mode) {
    f32 half;

    p->m_CharacterActor.m_AI.m_pParent = p;
    func_0021E6AC(p, rec, mode, 0);
    p->m_CharacterActor.m_AI.m_pParent = p;
    p->m_CharacterActor.m_dwFlags |= 0x100000;
    p->m_CharacterActor.m_dwFlags |= 0x10000;
    p->m_CharacterActor.m_dwFlags |= 0x1000000;
    p->m_CharacterActor.m_dwFlags |= 0x2000000;
    if (!D_8011AAD8.multiplayer) {
        p->m_CharacterActor.m_dwFlags |= 0x10000000;
    }
    p->m_CharacterActor.m_AI.m_pfCalculateOrientationMatrix = func_0021E840;
    p->m_CharacterActor.m_AI.m_pfEffects = func_00256738;
    p->m_CharacterActor.m_AI.m_pfHurt = func_00247C10;
    p->m_CharacterActor.m_AI.m_pfHit = func_0025689C;
    if (D_8011AAD8.multiplayer) {
        p->m_CharacterActor.m_AI.m_pfDraw = func_00247FB0;
    }

    p->m_WeaponActor.m_AI.m_pParent = p;
    func_0021E6AC(&p->m_WeaponActor, rec, mode, 0);
    p->m_WeaponActor.m_AI.m_pfCalculateOrientationMatrix = func_00257A40;
    p->m_WeaponActor.m_AI.m_Mode = -1;
    p->m_WeaponActor.m_AI.m_pParent = p;
    p->m_WeaponActor.m_AI.m_pfHurt = func_00247C10;
    p->m_WeaponActor.m_AI.m_pfHit = func_0025689C;
    p->m_WeaponActor.m_dwFlags |= 0x200000;
    p->m_Data.m_CurrentWeapon = -1;
    half = D_800A6A98;
    p->m_WeaponActor.m_vScale.x = half;
    p->m_WeaponActor.m_vScale.y = half;
    p->m_WeaponActor.m_vScale.z = half;
    p->m_WeaponActor.m_dwFlags &= ~0x10000;
    func_00243414(&p->m_WeaponActor, &p->m_WeaponActor.m_AI, 0);
    func_00258668(&p->m_WeaponActor, &p->m_WeaponActor.m_AI);
    func_0024CF58(p, p);
    func_00284254(p->m_pCamera);
    p->m_CharacterActor.m_vScale.x = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_ScaleX;
    p->m_CharacterActor.m_vScale.y = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_ScaleY;
    p->m_CharacterActor.m_vScale.z = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_ScaleZ;
}
