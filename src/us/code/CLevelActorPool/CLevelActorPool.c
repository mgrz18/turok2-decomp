#include "common.h"
#include "structs/vector.h"
#include "structs/level_actor.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/scene.h"

void CLevelActorPool__Free(CLevelActorPool *arg0, CPooledLevelActor *arg1);
f32 GameObject_GetCollisionHeight(CPooledLevelActor *actor);

/* Takes a free slot, or releases the oldest actor for one, links it at the head and clears it. */
static __inline__ CPooledLevelActor *allocActor(CLevelActorPool *pool) {
    CPooledLevelActor *actor;

    if (pool->m_pFree == NULL) {
        CLevelActorPool__Free(pool, pool->m_pTail);
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
CPooledLevelActor *CLevelActorPool__Spawn(CLevelActorPool *pool, s32 *refCount, s32 id, CVector3 vel, CVector3 pos, sptr arg9, f32 lifetime) {
    CPooledLevelActor *actor;
    CVector3 p;
    s32 index;
    s32 resource;

    if (gPersistentDataLoaded == 0 || (index = CScene__FindObjectType(&gScene, id)) == -1) {
        return NULL;
    }
    actor = allocActor(pool);
    resource = CScene__GetObject(&gScene, index);
    actor->m_pOwnerCount = refCount;
    if (refCount != NULL) {
        (*refCount)++;
    }
    if (gEngineApp_m_Settings.multiplayer) {
        actor->m_Actor.header.m_pIntelligence = &gLevelActorIntelligenceMP;
    } else {
        actor->m_Actor.header.m_pIntelligence = &gLevelActorIntelligence;
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
    actor->m_Actor.m_DrawnFrame = actor->m_Actor.m_UpdatedFrame = (u8)gFrameCount - 1;
    p.x = actor->m_Actor.header.m_vPos.x;
    p.y = actor->m_Actor.header.m_vPos.y + GameObject_GetCollisionHeight(actor) * 0.5f;
    p.z = actor->m_Actor.header.m_vPos.z;
    CScene__GetLightsAt(&gScene, &p, &actor->m_Actor.m_Lights);
    return actor;
}

#include "structs/collision.h"
#include "globals/camera.h"

void CPooledLevelActor__Update(CPooledLevelActor *obj);

/* Releases an actor in use: uncounts it, unlinks it and puts it on the free list. */
static __inline__ void releaseActor(CLevelActorPool *pool, CPooledLevelActor *actor) {
    if (actor->m_PoolFlags & 1) {
        actor->m_PoolFlags &= ~1;
        if (actor->m_pOwnerCount != NULL) {
            (*actor->m_pOwnerCount)--;
        }
        if (actor->m_pPrev != NULL) {
            actor->m_pPrev->m_pNext = actor->m_pNext;
        }
        if (actor->m_pNext != NULL) {
            actor->m_pNext->m_pPrev = actor->m_pPrev;
        }
        if (pool->m_pHead == actor) {
            pool->m_pHead = actor->m_pNext;
        }
        if (pool->m_pTail == actor) {
            pool->m_pTail = actor->m_pPrev;
        }
        actor->m_pNext = pool->m_pFree;
        actor->m_pPrev = NULL;
        pool->m_pFree = actor;
    }
}

/* Updates the pool actors (CPooledLevelActor__Update); releases those whose lifetime ran out this frame
 * or that left the visible bounds. */
void CLevelActorPool__Update(CLevelActorPool *pool) {
    CPooledLevelActor *actor;
    CPooledLevelActor *next;
    s32 expired;

    next = pool->m_pHead;
    while (next != NULL) {
        actor = next;
        next = next->m_pNext;
        expired = 0;
        if (actor->m_Lifetime > 0.0f) {
            actor->m_Lifetime -= gFrameIncrement;
            if (actor->m_Lifetime <= 0.0f) {
                expired = 1;
            }
        }
        if (!(gCameraVisibleBox.m_vMax.x > actor->m_Actor.m_Bounds.m_vMin.x && gCameraVisibleBox.m_vMin.x < actor->m_Actor.m_Bounds.m_vMax.x
              && gCameraVisibleBox.m_vMax.z > actor->m_Actor.m_Bounds.m_vMin.z && gCameraVisibleBox.m_vMin.z < actor->m_Actor.m_Bounds.m_vMax.z
              && gCameraVisibleBox.m_vMax.y > actor->m_Actor.m_Bounds.m_vMin.y && gCameraVisibleBox.m_vMin.y < actor->m_Actor.m_Bounds.m_vMax.y)) {
            expired = 1;
        }
        if (expired) {
            releaseActor(pool, actor);
        } else {
            CPooledLevelActor__Update(actor);
        }
    }
}

#include "structs/game_object.h"

void SceneActor_Init(CGameObjectInstance *arg0) {
    arg0->header.m_pIntelligence = &gLevelActorIntelligence;
    arg0->header.m_Type = 3;
    arg0->m_AI.m_RotYLastFrame = 0.0f;
}

#include "globals/collision.h"

void CLevelActor__Update(CPooledLevelActor *obj);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Collision_MoveInstance Collision_MoveInstance_f3
#endif

void Collision_MoveInstance(CPooledLevelActor *obj, f32 x, f32 y, f32 z, void *arg4);

/* Counts the timer down, moves the object (CLevelActor__Update), stops it unless its definition is
 * gLevelActorIntelligence, places it at its position and refreshes its bounding box, 81.92 each way. */
void CPooledLevelActor__Update(CPooledLevelActor *obj) {
    CVector3 prev;

    if (obj->m_Timer > 0.0f) {
        obj->m_Timer -= gFrameIncrement;
        prev = obj->m_Actor.header.m_vPos;
    }
    CLevelActor__Update(obj);
    if (obj->m_Actor.header.m_pIntelligence != &gLevelActorIntelligence) {
        obj->m_Actor.m_vVelocity.x = 0.0f;
        obj->m_Actor.m_vVelocity.y = 0.0f;
        obj->m_Actor.m_vVelocity.z = 0.0f;
    }
    Collision_MoveInstance(obj, obj->m_Actor.header.m_vPos.x, obj->m_Actor.header.m_vPos.y, obj->m_Actor.header.m_vPos.z, &gCollisionInfoPooledActor);
    obj->m_Actor.m_Bounds.m_vMin.x = obj->m_Actor.header.m_vPos.x - 81.92f;
    obj->m_Actor.m_Bounds.m_vMin.y = obj->m_Actor.header.m_vPos.y - 81.92f;
    obj->m_Actor.m_Bounds.m_vMin.z = obj->m_Actor.header.m_vPos.z - 81.92f;
    obj->m_Actor.m_Bounds.m_vMax.x = obj->m_Actor.header.m_vPos.x + 81.92f;
    obj->m_Actor.m_Bounds.m_vMax.y = obj->m_Actor.header.m_vPos.y + 81.92f;
    obj->m_Actor.m_Bounds.m_vMax.z = obj->m_Actor.header.m_vPos.z + 81.92f;
}

/* Unlinks an active node and pushes it on the free list. */
void CPooledLevelActor__Release(CPooledLevelActor *arg0) {
    s32 *temp_v1;
    s32 temp_a1;
    CPooledLevelActor *temp_v1_2;
    CPooledLevelActor *temp_v1_3;
    CPooledLevelActor **nextp;

    temp_a1 = arg0->m_PoolFlags;
    if (temp_a1 & 1) {
        temp_v1 = arg0->m_pOwnerCount;
        arg0->m_PoolFlags = (s32) (temp_a1 & ~1);
        if (temp_v1 != NULL) {
            *temp_v1 -= 1;
        }
        temp_v1_2 = arg0->m_pPrev;
        if (temp_v1_2 != NULL) {
            temp_v1_2->m_pNext = arg0->m_pNext;
        }
        temp_v1_3 = arg0->m_pNext;
        if (temp_v1_3 != NULL) {
            temp_v1_3->m_pPrev = arg0->m_pPrev;
        }
        if (gScene_m_ActorPool_m_pHead == arg0) {
            gScene_m_ActorPool_m_pHead = arg0->m_pNext;
        }
        if (gScene_m_ActorPool_m_pTail == arg0) {
            gScene_m_ActorPool_m_pTail = arg0->m_pPrev;
        }
        nextp = &arg0->m_pNext;
        *nextp = gScene_m_ActorPool_m_pFree;
        arg0->m_pPrev = NULL;
        gScene_m_ActorPool_m_pFree = arg0;
    }
}

#include "structs/light.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028FD2C_12.CPP, func_802905F4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */


/* Turok 2 layout (Rage Wars offsets in parentheses). */
typedef struct SceneEffect {
    char pad0[0x14C];
    f32 minX;                  /* 0x14C (0x17C) */
    f32 minY;                  /* 0x150 */
    f32 minZ;                  /* 0x154 */
    f32 maxX;                  /* 0x158 */
    f32 maxY;                  /* 0x15C */
    f32 maxZ;                  /* 0x160 */
    char pad164[0x198 - 0x164];
    f32 lifetime;              /* 0x198 (0x1C8) */
    char pad19C[0x1AC - 0x19C];
    struct SceneEffect *next;  /* 0x1AC (0x1DC) */
} SceneEffect;
typedef struct GameplayCamera {
    char pad0[0x2D8];
    char clipPlanes[0x60];     /* 0x2D8 (0x2F0) */
    CVector3 clipBounds[2];    /* 0x338 (0x350) */
} GameplayCamera;
typedef struct WorldRecAt3C08 {
    char pad0[0x3604];
    SceneEffect *head;         /* 0x3604 (0x3C04) */
} WorldRecAt3C08;
extern void Render_ResetBatches();
extern void Render_FlushBatches();
extern s32 Frustum_TestBox(void *arg0, void *arg1);
extern void CLevelActor__Draw(void *arg0);

/* Draw the scene effects whose bounds are inside the camera's clip volume. */
void CLevelActorPool__Draw(char *arg0, char *arg1) {
    WorldRecAt3C08 *world = (void *)arg0;
    GameplayCamera *camera = (void *)arg1;
    SceneEffect *entry;
    f32 value;
    f32 upper;
    f32 scale;
    f32 zero;
    Render_ResetBatches();
    entry = world->head;
    if (entry != 0) {
        zero = 0.0f;
        upper = 7.5f;
        scale = 34.0f;
        do {
            if ((camera->clipBounds[1].x > entry->minX) &&
                (camera->clipBounds[0].x < entry->maxX) &&
                (camera->clipBounds[1].z > entry->minZ) &&
                (camera->clipBounds[0].z < entry->maxZ) &&
                (camera->clipBounds[1].y > entry->minY) &&
                (camera->clipBounds[0].y < entry->maxY) &&
                (Frustum_TestBox(camera->clipPlanes, &entry->minX) != 0)) {
                value = entry->lifetime;
                gBlendState = 0;
                if ((value > zero) && (value <= upper)) {
                    gBlendState = 1;
                    gBlendState_m_Color[3] = value * scale;
                }
                CLevelActor__Draw(entry);
                gBlendState = 0;
            }
            entry = entry->next;
        } while (entry != 0);
    }
    Render_FlushBatches();
}

#include "structs/world.h"

s32 GameObject_IsSolid(CPooledLevelActor *actor);

/* Adds to the world's draw list the pool actors with flags 0x20000 (of 0x30000) that
 * GameObject_IsSolid accepts and that are inside the visible bounds. */
void CLevelActorPool__CollectVisible(CLevelActorPool *pool, CScene *world) {
    CPooledLevelActor *actor;

    for (actor = pool->m_pHead; actor != NULL; actor = actor->m_pNext) {
        if ((*(s32 *) &actor->m_Actor.m_wFlags & 0x30000) == 0x20000 && GameObject_IsSolid(actor)
            && gCameraVisibleBox.m_vMax.x > actor->m_Actor.m_Bounds.m_vMin.x && gCameraVisibleBox.m_vMin.x < actor->m_Actor.m_Bounds.m_vMax.x
            && gCameraVisibleBox.m_vMax.z > actor->m_Actor.m_Bounds.m_vMin.z && gCameraVisibleBox.m_vMin.z < actor->m_Actor.m_Bounds.m_vMax.z
            && gCameraVisibleBox.m_vMax.y > actor->m_Actor.m_Bounds.m_vMin.y && gCameraVisibleBox.m_vMin.y < actor->m_Actor.m_Bounds.m_vMax.y) {
            if (world->m_nDrawActors != 0x200) {
                world->m_pDrawActors[world->m_nDrawActors++] = (CGameObjectInstance *)actor;
            }
        }
    }
}

CPooledLevelActor *CLevelActorPool__Alloc(CLevelActorPool *pool) {
    CPooledLevelActor *node;

    if (pool->m_pFree == NULL) {
        CLevelActorPool__Free(pool, pool->m_pTail);
    }
    node = pool->m_pFree;
    pool->m_pFree = node->m_pNext;
    if (pool->m_pHead != NULL) {
        pool->m_pHead->m_pPrev = node;
    }
    node->m_pNext = pool->m_pHead;
    node->m_pPrev = NULL;
    pool->m_pHead = node;
    if (pool->m_pTail == NULL) {
        pool->m_pTail = node;
    }
    node->m_Actor.m_vVelocity.x = 0;
    node->m_Actor.m_vVelocity.y = 0;
    node->m_Actor.m_vVelocity.z = 0;
    node->m_Actor.unk24[0] = 0;
    node->m_Actor.unk24[1] = 0;
    node->m_Actor.unk24[2] = 0;
    node->m_Actor.unk24[3] = 0;
    node->m_Lifetime = 0;
    node->m_PoolFlags |= 1;
    return node;
}

/* Releases arg1 back to the pool's free list. */
void CLevelActorPool__Free(CLevelActorPool *arg0, CPooledLevelActor *arg1) {
    s32 *temp_v1;
    s32 temp_a2;
    CPooledLevelActor *temp_v1_2;
    CPooledLevelActor *temp_v1_3;

    temp_a2 = arg1->m_PoolFlags;
    if (temp_a2 & 1) {
        temp_v1 = arg1->m_pOwnerCount;
        arg1->m_PoolFlags = temp_a2 & ~1;
        if (temp_v1 != NULL) {
            *temp_v1 -= 1;
        }
        temp_v1_2 = arg1->m_pPrev;
        if (temp_v1_2 != NULL) {
            temp_v1_2->m_pNext = arg1->m_pNext;
        }
        temp_v1_3 = arg1->m_pNext;
        if (temp_v1_3 != NULL) {
            temp_v1_3->m_pPrev = arg1->m_pPrev;
        }
        if (arg0->m_pHead == arg1) {
            arg0->m_pHead = arg1->m_pNext;
        }
        if (arg0->m_pTail == arg1) {
            arg0->m_pTail = arg1->m_pPrev;
        }
        arg1->m_pNext = arg0->m_pFree;
        arg1->m_pPrev = NULL;
        arg0->m_pFree = arg1;
    }
}
