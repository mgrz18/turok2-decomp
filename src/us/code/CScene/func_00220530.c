#include "common.h"
#include "protos/cache.h"
#include "globals/swoosh.h"
#include "structs/world.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/math.h"
#include "globals/engine.h"
#include "globals/scene.h"
#include "structs/vector.h"
/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
void CScene__AdvanceInstances(void *);
void CScene__AdvanceListedInstances(void *);
void CScene__BuildActorLists(struct CScene *);
void CScene__ClearListCounts(void *);
void CScene__CollectSectorObjects(struct CScene *);
void CScene__DrawWarpFade(struct CScene *);
void CScene__LoadVisibleGridSections(struct CScene *);
void CScene__ReleaseSectors(struct CScene *);
void CScene__Update(struct CScene *);
void CScene__UpdateDelayedEvents(struct CScene *);
void CScene__UpdateWarp(struct CScene *);

/* The input state the controller reader fills (gEngineApp_m_Settings, 0x184 bytes). */

/* The camera pool as bytes (globals/camera.h declares the CCameraPool): this view keeps the ROM's code. */
extern u8 gCameraPoolBytes[] __asm__("gEngineApp_m_Game_m_CameraPool");

void MtxPool_Reset(void *pool);
void RibbonVtxPool_Reset(void *);
void CPlayerPool__PreUpdate(void *);
void CPlayerPool__AnimateWeapons(void *);
void CCameraPool__Draw(void *, CScene *);
void CParticleSystem__Update(CParticleSystem *);
void SwooshSystem_Update(void *);
void CLevelActorPool__Update(CLevelActorPool *);
f32 func_002982F0(f32);     /* sinf */
f32 func_002974C0(f32);     /* cosf */

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
        ((void (*)(s32, s32)) CCache__SetFrameDelay)(8, old); /* the definition takes one argument */
    } else {
        gLitVerticesKeyChanged = 0;
    }
    CScene__UpdateWarp(world);
    CScene__ClearListCounts(world);
    MtxPool_Reset(world->m_Heap);
    CCache__LockForDisplayList(0, world->m_hHeap);
    RibbonVtxPool_Reset(&gEngineApp_m_SwooshSystem);
    CPlayerPool__PreUpdate(&gEngineApp_m_Game.m_PlayerList);
    CScene__BuildActorLists(world);
    CScene__LoadVisibleGridSections(world);
    CScene__CollectSectorObjects(world);
    if (world->m_WarpState == 0) {
        CPlayerPool__AnimateWeapons(&gEngineApp_m_Game.m_PlayerList);
    }
    CCameraPool__Draw(gCameraPoolBytes, world);
    CParticleSystem__Update(&world->m_ParticleSystem);
#ifdef TARGET_PORT
    /* The port: GameSettings.paused by member (CGame + 0x40 + 0x13F4 on the N64; on the host the
     * literal offset read inside the camera pool and every level stood still: no AdvanceInstances,
     * no CPlayerPool__Update, no player input). */
    if (world->m_WarpState == 0 && gEngineApp_m_Settings.paused == 0) {
#else
    if (world->m_WarpState == 0 && *(s32 *)(gCameraPoolBytes + 0x13F4) == 0) {
#endif
        SwooshSystem_Update(&gEngineApp_m_SwooshSystem);
        CScene__AdvanceInstances(world);
        CScene__AdvanceListedInstances(world);
        CLevelActorPool__Update(&world->m_ActorPool);
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
