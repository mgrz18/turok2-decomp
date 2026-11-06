#include "common.h"
#include "structs/world.h"
#include "structs/game.h"

/* The input state the controller reader fills (D_8011AAD8, 0x184 bytes). */

extern s32 D_800ACAD8;      /* buttons this frame */
extern s32 D_800ACADC;      /* buttons changed */
extern s32 D_800ACDE0;
extern f32 D_800B6D28;      /* frame time scale */
extern f32 D_800DD78C;
extern u8 D_801102B8[];
extern u8 D_801198B0[];

void CCache__SetFrameDelay(s32, s32);
void CScene__UpdateWarp(CWorld *);
void CScene__ClearListCounts(CWorld *);
void MtxPool_Reset(void *);
void CCache__LockForDisplayList(s32, void **);
void RibbonVtxPool_Reset(void *);
void CPlayerPool__PreUpdate(void *);
void CScene__BuildActorLists(CWorld *);
void CScene__LoadVisibleGridSections(CWorld *);
void CScene__CollectSectorObjects(CWorld *);
void CPlayerPool__AnimateWeapons(void *);
void CCameraPool__Draw(void *, CWorld *);
void CParticleSystem__Update(void *);
void SwooshSystem_Update(void *);
void CScene__AdvanceInstances(CWorld *);
void CScene__AdvanceListedInstances(CWorld *);
void CLevelActorPool__Update(void *);
f32 func_002982F0(f32);     /* sinf */
f32 func_002974C0(f32);     /* cosf */
void CScene__UpdateDelayedEvents(CWorld *);
void CScene__ReleaseSectors(CWorld *);
void CScene__DrawWarpFade(CWorld *);

void CScene__Update(CWorld *world) {
    GameSettings *input;
    s32 old;
    f32 s;

    old = D_800ACAD8;
    input = &D_8011AAD8;
    D_800ACDE0 = 0;
    D_800ACAD8 = input->unkC;
    if (input->cheatFlags & 0x4000) {
        D_800ACAD8 |= 0x100;
    }
    if (input->cheatFlags & 0x8000) {
        D_800ACAD8 |= 0x200;
    }
    if (old != D_800ACAD8) {
        D_800ACADC = 1;
        CCache__SetFrameDelay(8, old);
    } else {
        D_800ACADC = 0;
    }
    CScene__UpdateWarp(world);
    CScene__ClearListCounts(world);
    MtxPool_Reset(world->m_Heap);
    CCache__LockForDisplayList(0, world->m_hHeap);
    RibbonVtxPool_Reset(D_801102B8);
    CPlayerPool__PreUpdate(&D_80119870.m_PlayerList);
    CScene__BuildActorLists(world);
    CScene__LoadVisibleGridSections(world);
    CScene__CollectSectorObjects(world);
    if (world->m_WarpState == 0) {
        CPlayerPool__AnimateWeapons(&D_80119870.m_PlayerList);
    }
    CCameraPool__Draw(D_801198B0, world);
    CParticleSystem__Update(world->m_ParticleSystem);
    if (world->m_WarpState == 0 && *(s32 *)(D_801198B0 + 0x13F4) == 0) {
        SwooshSystem_Update(D_801102B8);
        CScene__AdvanceInstances(world);
        CScene__AdvanceListedInstances(world);
        CLevelActorPool__Update(world->m_ActorPool);
        world->m_SkyAngle += D_800B6D28 * 0.087266475f;
        s = func_002982F0(world->m_SkyAngle * 0.5f);
        D_800DD78C = s;
        world->m_qSky.x = 0;
        world->m_qSky.y = s;
        world->m_qSky.z = 0;
        world->m_qSky.w = func_002974C0(world->m_SkyAngle * 0.5f);
        CScene__UpdateDelayedEvents(world);
    }
    CScene__ReleaseSectors(world);
    CScene__DrawWarpFade(world);
}
