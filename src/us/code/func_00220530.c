#include "common.h"
#include "structs/world.h"
#include "structs/game.h"
#include "globals/game.h"

/* The input state the controller reader fills (gEngineApp_m_Settings, 0x184 bytes). */

extern s32 gLitVerticesKey;      /* lit-vertices cache key (GameSettings+0xC, cheat bits 0x100/0x200) */
extern s32 gLitVerticesKeyChanged;      /* buttons changed */
extern s32 gSceneGeometryDL;
extern f32 gFrameIncrement;      /* frame time scale */
extern f32 gQuaternionSin;
extern u8 gEngineApp_m_SwooshSystem[];
extern u8 gEngineApp_m_Game_m_CameraPool[]; /* local type: globals/camera.h declares it; this view (its type here) is what keeps the ROM's code */

void CCache__SetFrameDelay(s32, s32);
void CScene__UpdateWarp(CScene *);
void CScene__ClearListCounts(CScene *);
void MtxPool_Reset(void *);
void CCache__LockForDisplayList(s32, void **);
void RibbonVtxPool_Reset(void *);
void CPlayerPool__PreUpdate(void *);
void CScene__BuildActorLists(CScene *);
void CScene__LoadVisibleGridSections(CScene *);
void CScene__CollectSectorObjects(CScene *);
void CPlayerPool__AnimateWeapons(void *);
void CCameraPool__Draw(void *, CScene *);
void CParticleSystem__Update(void *);
void SwooshSystem_Update(void *);
void CScene__AdvanceInstances(CScene *);
void CScene__AdvanceListedInstances(CScene *);
void CLevelActorPool__Update(void *);
f32 func_002982F0(f32);     /* sinf */
f32 func_002974C0(f32);     /* cosf */
void CScene__UpdateDelayedEvents(CScene *);
void CScene__ReleaseSectors(CScene *);
void CScene__DrawWarpFade(CScene *);

void CScene__Update(CScene *world) {
    GameSettings *input;
    s32 old;
    f32 s;

    old = gLitVerticesKey;
    input = &gEngineApp_m_Settings;
    gSceneGeometryDL = 0;
    gLitVerticesKey = input->unkC;
    if (input->cheatFlags & 0x4000) {
        gLitVerticesKey |= 0x100;
    }
    if (input->cheatFlags & 0x8000) {
        gLitVerticesKey |= 0x200;
    }
    if (old != gLitVerticesKey) {
        gLitVerticesKeyChanged = 1;
        CCache__SetFrameDelay(8, old);
    } else {
        gLitVerticesKeyChanged = 0;
    }
    CScene__UpdateWarp(world);
    CScene__ClearListCounts(world);
    MtxPool_Reset(world->m_Heap);
    CCache__LockForDisplayList(0, world->m_hHeap);
    RibbonVtxPool_Reset(gEngineApp_m_SwooshSystem);
    CPlayerPool__PreUpdate(&gEngineApp_m_Game.m_PlayerList);
    CScene__BuildActorLists(world);
    CScene__LoadVisibleGridSections(world);
    CScene__CollectSectorObjects(world);
    if (world->m_WarpState == 0) {
        CPlayerPool__AnimateWeapons(&gEngineApp_m_Game.m_PlayerList);
    }
    CCameraPool__Draw(gEngineApp_m_Game_m_CameraPool, world);
    CParticleSystem__Update(world->m_ParticleSystem);
    if (world->m_WarpState == 0 && *(s32 *)(gEngineApp_m_Game_m_CameraPool + 0x13F4) == 0) {
        SwooshSystem_Update(gEngineApp_m_SwooshSystem);
        CScene__AdvanceInstances(world);
        CScene__AdvanceListedInstances(world);
        CLevelActorPool__Update(world->m_ActorPool);
        world->m_SkyAngle += gFrameIncrement * 0.087266475f;
        s = func_002982F0(world->m_SkyAngle * 0.5f);
        gQuaternionSin = s;
        world->m_qSky.x = 0;
        world->m_qSky.y = s;
        world->m_qSky.z = 0;
        world->m_qSky.w = func_002974C0(world->m_SkyAngle * 0.5f);
        CScene__UpdateDelayedEvents(world);
    }
    CScene__ReleaseSectors(world);
    CScene__DrawWarpFade(world);
}
