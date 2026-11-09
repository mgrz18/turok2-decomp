/* Second VM module copy of CPlayer__LevelStart (same source, linked 0x0042DD94). */
#include "common.h"
#include "structs/game.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/render.h"
#include "globals/player.h"
#include "protos/game.h"

extern void CPlayer__PickSpawnPoint(void);
extern void CPlayer__LoadCharacterModel(CPlayer *p);
extern void COverlay__Construct(COverlay *item, s32 x, s32 y);
extern void COverlay__StartScript(COverlay *item, s32 *script);
extern void CPlayer__Reset(CPlayer *p);
extern s32 Cinema_IsActive(void);
extern s16 *CPlayer__GetFiringAmmo(CPlayer *p, s32);
extern void Cinema_Disable(void);
extern void Cinema_Enable(void);
extern void CGameRegion__FireTriggers(s32, s32, CPlayer *p);

void CPlayer__LevelStart_2(CPlayer *p) {
    GameSettings *flags = &gEngineApp_m_Settings;
    CPlayerOverlay *c;
    s32 warp;

    if (flags->multiplayer) {
        CPlayer__PickSpawnPoint();
        CPlayer__LoadCharacterModel(p);
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
    c->m_vPos.y = 437.5f;
    c->m_vPos.z = -675.0f;
    c->m_vScale.x = 0.25f;
    c->m_vScale.y = 0.1875f;
    c->m_vScale.z = 0.25f;
    c->m_vRot.y = 1.5707965f;
    p->m_pRumble = NULL;
    if (!flags->multiplayer) {
        Controller_SetRumbleOption(p->m_ContState.m_pContState, 1);
    }
    p->m_FlashlightTime = 0.0f;
    p->m_pDLFlashlight1 = NULL;
    p->m_pDLFlashlight2 = NULL;
    p->m_pDLDarkCityLight = NULL;
    COverlay__Construct(&p->m_AmmoOverlay, 120, 96);
    COverlay__StartScript(&p->m_AmmoOverlay, gAmmoOverlayScript);
    COverlay__Construct(&p->m_HealthOverlay, 48, 96);
    COverlay__StartScript(&p->m_HealthOverlay, gHealthOverlayScript);
    COverlay__Construct(&p->m_LivesOverlay, -64, 16);
    COverlay__StartScript(&p->m_LivesOverlay, gLivesOverlayScript);
    COverlay__Construct(&p->m_TokensOverlay, 40, 16);
    COverlay__StartScript(&p->m_TokensOverlay, gTokensOverlayScript);
    COverlay__Construct(&p->m_TotalKillsOverlay, 40, 42);
    COverlay__StartScript(&p->m_TotalKillsOverlay, gTotalKillsOverlayScript);
    COverlay__Construct(&p->m_TotalTagsOverlay, 40, 68);
    COverlay__StartScript(&p->m_TotalTagsOverlay, gTotalTagsOverlayScript);
    COverlay__Construct(&p->m_AirOverlay, 24, (s16) (gScreenHeight + 96));
    warp = 0;
    COverlay__Construct(&p->m_BossOverlay, 24, (s16) (gScreenHeight + 96));
    p->m_SfxTimer = 0.0f;
    p->m_SightIconFrame = 0.0f;
    p->PreviousCameraQuatern = p->m_pCamera->m_qViewAlign;
    CPlayer__Reset(p);
    if (!Cinema_IsActive()) {
        warp = gScene_m_LevelID != 999;
    }
    if (gScene.m_bStreamAll) {
        warp = 1;
    }
    if (warp) {
        p->m_Data.m_CheckPointWarpID = gScene_m_LevelID;
    }
    if (!flags->multiplayer && p->m_Data.m_WeaponList[21]) {
        /* the ROM adds the scaled index first: written index + array */
        *(*CPlayer__GetFiringAmmo(p, 21) + p->m_Data.m_AmmoList) = 1;
    }
    Cinema_Disable();
    if (p->m_CharacterActor.header.m_pCurrentRegion) {
        CGameRegion__FireTriggers((sptr) p->m_CharacterActor.header.m_pCurrentRegion, 1, p);
    }
    Cinema_Enable();
}
