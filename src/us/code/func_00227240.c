#include "common.h"
#include "structs/vector.h"
#include "structs/level_actor.h"

extern s32 D_800B6D54;
extern u8 D_800F7078[];                 /* the world */
extern u8 D_8011AAF1;
extern u8 D_800F12C0[];
extern u8 D_800F1360[];
extern u8 D_800B6D1B;                   /* frame counter */

s32 func_00224180(void *world, s32 id);
s32 func_00224F84(void *world, s32 index);
void func_00227B7C(CLevelActorPool *pool, CPooledLevelActor *actor);
f32 func_002671B4(CPooledLevelActor *actor);
void func_002254C0(void *world, CVector3 *pos, void *out);

/* Takes a free slot, or releases the oldest actor for one, links it at the head and clears it. */
static __inline__ CPooledLevelActor *allocActor(CLevelActorPool *pool) {
    CPooledLevelActor *actor;

    if (pool->m_pFree == NULL) {
        func_00227B7C(pool, pool->m_pTail);
    }
    actor = pool->m_pFree;
    pool->m_pFree = actor->m_pNext;
    if (pool->m_pHead != NULL) {
        pool->m_pHead->m_pPrev = actor;
    }
    actor->m_pNext = pool->m_pHead;
    actor->m_pPrev = NULL;
    pool->m_pHead = actor;
    if (pool->m_pTail == NULL) {
        pool->m_pTail = actor;
    }
    actor->m_Actor.m_vVelocity.x = 0;
    actor->m_Actor.m_vVelocity.y = 0;
    actor->m_Actor.m_vVelocity.z = 0;
    actor->m_Actor.unk24[0] = 0;
    actor->m_Actor.unk24[1] = 0;
    actor->m_Actor.unk24[2] = 0;
    actor->m_Actor.unk24[3] = 0;
    actor->m_Lifetime = 0;
    actor->m_PoolFlags |= 1;
    return actor;
}

/* Spawns a pool actor of the given level id at pos with velocity vel, taking a free slot (or
 * the oldest one, released first) and linking it at the head; counts it in *refCount. */
CPooledLevelActor *func_00227240(CLevelActorPool *pool, s32 *refCount, s32 id, CVector3 vel, CVector3 pos, s32 arg9, f32 lifetime) {
    CPooledLevelActor *actor;
    CVector3 p;
    s32 index;
    s32 resource;

    if (D_800B6D54 == 0 || (index = func_00224180(D_800F7078, id)) == -1) {
        return NULL;
    }
    actor = allocActor(pool);
    resource = func_00224F84(D_800F7078, index);
    actor->m_pOwnerCount = refCount;
    if (refCount != NULL) {
        (*refCount)++;
    }
    if (D_8011AAF1) {
        actor->m_Actor.header.m_pIntelligence = D_800F12C0;
    } else {
        actor->m_Actor.header.m_pIntelligence = D_800F1360;
    }
    actor->m_Lifetime = lifetime;
    actor->m_Timer = 0.0f;
    actor->m_Actor.header.m_nObjType = index;
    actor->m_Actor.header.m_vPos = pos;
    actor->m_Actor.m_RotY = 0;
    actor->m_Actor.unk148 = 0;
    actor->m_Actor.unk138 = 0;
    actor->m_Actor.unk140 = 0;
    actor->m_Actor.m_Scale = 1.0f;
    actor->m_Actor.m_Bounds.m_vMin.x = pos.x - 81.92f;
    actor->m_Actor.m_Bounds.m_vMin.y = pos.y - 81.92f;
    actor->m_Actor.m_Bounds.m_vMin.z = pos.z - 81.92f;
    actor->m_Actor.m_Bounds.m_vMax.x = pos.x + 81.92f;
    actor->m_Actor.m_Bounds.m_vMax.y = pos.y + 81.92f;
    actor->m_Actor.m_Bounds.m_vMax.z = pos.z + 81.92f;
    actor->m_Actor.m_Resource = resource;
    actor->m_Actor.header.m_pCurrentRegion = (struct CGameRegion *) arg9;
    actor->m_Actor.m_wFlags = 0x1A;
    actor->m_Actor.m_ModelScale = 0;
    actor->m_Actor.m_vVelocity = vel;
    actor->m_Actor.m_DrawnFrame = actor->m_Actor.unk195 = D_800B6D1B - 1;
    p.x = actor->m_Actor.header.m_vPos.x;
    p.y = actor->m_Actor.header.m_vPos.y + func_002671B4(actor) * 0.5f;
    p.z = actor->m_Actor.header.m_vPos.z;
    func_002254C0(D_800F7078, &p, actor->m_Actor.m_Lights);
    return actor;
}
