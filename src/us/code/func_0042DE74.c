/* CPlayer code: func_0042DE74, 0x0042DE74-0x0042E14C (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/game.h"
#include "structs/player.h"
#include "structs/camera.h"

extern s32 D_8011001C;
extern s32 D_80110048;
extern s32 D_800C2034;          /* screen height */
extern u8 D_800ADB60[];
extern u8 D_800ADB74[];
extern u8 D_800ADBB0[];
extern u8 D_800ADBF0[];
extern u8 D_800ADC30[];
extern u8 D_800ADC70[];
/* In the engine's .rodata, the pool shared with func_00248720. */
extern f32 D_800A6AA0;          /* 437.5f */
extern f32 D_800A6AA4;          /* -675.0f */
extern f32 D_800A6AA8;          /* 0.25f */
extern f32 D_800A6AAC;          /* 0.1875f */
extern f32 D_800A6AB0;          /* 1.5707964f */

extern void func_00254044(void);
extern void func_002541E4(CPlayer *p);
extern void func_00285BD0(s32 controller, s32 state);
extern void func_0027ACC4(COverlay *item, s32 x, s32 y);
extern void func_0027AD00(COverlay *item, u8 *anim);
extern void func_00248340(CPlayer *p);
extern s32 func_00284174(void);
extern s16 *func_0025398C(CPlayer *p, s32);
extern void func_002842C8(void);
extern void func_002842B4(void);
extern void func_0026E19C(s32, s32, CPlayer *p);

void func_0042DE74(CPlayer *p) {
    GameSettings *flags = &D_8011AAD8;
    CPlayerOverlay *c;
    s32 warp;

    if (flags->multiplayer) {
        func_00254044();
        func_002541E4(p);
    }
    c = &p->m_Overlay;
    c->m_Mode = 0;
    c->m_rpObject = NULL;
    c->m_ModeTime = 0.0f;
    c->m_DesiredObjectType = -1;
    c->m_ObjectType = -1;
    c->m_vPos.x = 0.0f;
    c->m_vRot.x = 0.0f;
    c->m_vRot.z = 0.0f;
    c->m_vPos.y = D_800A6AA0;
    c->m_vPos.z = D_800A6AA4;
    c->m_vScale.x = D_800A6AA8;
    c->m_vScale.y = D_800A6AAC;
    c->m_vScale.z = D_800A6AA8;
    c->m_vRot.y = D_800A6AB0;
    p->m_pRumble = NULL;
    if (!flags->multiplayer) {
        func_00285BD0((s32) p->m_ContState.m_pContState, 1);
    }
    p->m_FlashlightTime = 0.0f;
    p->m_pDLFlashlight1 = NULL;
    p->m_pDLFlashlight2 = NULL;
    p->m_pDLDarkCityLight = NULL;
    func_0027ACC4(&p->m_AmmoOverlay, 120, 96);
    func_0027AD00(&p->m_AmmoOverlay, D_800ADB74);
    func_0027ACC4(&p->m_HealthOverlay, 48, 96);
    func_0027AD00(&p->m_HealthOverlay, D_800ADB60);
    func_0027ACC4(&p->m_LivesOverlay, -64, 16);
    func_0027AD00(&p->m_LivesOverlay, D_800ADBB0);
    func_0027ACC4(&p->m_TokensOverlay, 40, 16);
    func_0027AD00(&p->m_TokensOverlay, D_800ADBF0);
    func_0027ACC4(&p->m_TotalKillsOverlay, 40, 42);
    func_0027AD00(&p->m_TotalKillsOverlay, D_800ADC30);
    func_0027ACC4(&p->m_TotalTagsOverlay, 40, 68);
    func_0027AD00(&p->m_TotalTagsOverlay, D_800ADC70);
    func_0027ACC4(&p->m_AirOverlay, 24, (s16) (D_800C2034 + 96));
    warp = 0;
    func_0027ACC4(&p->m_BossOverlay, 24, (s16) (D_800C2034 + 96));
    p->m_SfxTimer = 0.0f;
    p->m_SightIconFrame = 0.0f;
    p->PreviousCameraQuatern = p->m_pCamera->m_qViewAlign;
    func_00248340(p);
    if (!func_00284174()) {
        warp = D_80110048 != 999;
    }
    if (D_8011001C) {
        warp = 1;
    }
    if (warp) {
        p->m_Data.m_CheckPointWarpID = D_80110048;
    }
    if (!flags->multiplayer && p->m_Data.m_WeaponList[21]) {
        /* the ROM adds the scaled index first: written index + array */
        *(*func_0025398C(p, 21) + p->m_Data.m_AmmoList) = 1;
    }
    func_002842C8();
    if (p->m_CharacterActor.header.m_pCurrentRegion) {
        func_0026E19C((s32) p->m_CharacterActor.header.m_pCurrentRegion, 1, p);
    }
    func_002842B4();
}
