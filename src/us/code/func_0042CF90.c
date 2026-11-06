/* WRONG CUT: REAL SceneActorPool_Construct size 0x110 (splat cut it into SceneActorPool_Construct + func_0042CFC0 + func_0042D040). */
#include "common.h"
#include "structs/intelligence.h"
#include "globals/scene.h"

/* A scene actor (0x1B0 bytes). */
typedef struct SceneActor {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ struct SceneActor *prev;
    /* 0x1AC */ struct SceneActor *next;
} SceneActor;

typedef struct ScenePool {
    /* 0x0000 */ SceneActor actors[32];
    /* 0x3600 */ SceneActor *free;
    /* 0x3604 */ SceneActor *head;
    /* 0x3608 */ s32 count;
} ScenePool;


extern void func_0027BD10(CIntelligence *def);
extern void func_00227664(SceneActor *actor);

/* Links the 32 actors of the pool into its free list and sets up the two default definitions. */
void SceneActorPool_Construct(ScenePool *pool) {
    s32 i;

    pool->free = pool->actors;
    pool->count = 0;
    pool->head = NULL;
    for (i = 0; i < 31; i++) {
        pool->actors[i].next = &pool->actors[i + 1];
        pool->actors[i].prev = &pool->actors[i - 1];
    }
    pool->actors[0].prev = NULL;
    pool->actors[31].next = NULL;

    func_0027BD10(&gLevelActorIntelligence);
    gLevelActorIntelligence.m_Type = 8;
    gLevelActorIntelligence.m_dwTypeFlags = 1;
    gLevelActorIntelligence.m_nTexture = 0;
    gLevelActorIntelligence.m_dwFlags |= 4;

    func_0027BD10(&gLevelActorIntelligenceMP);
    gLevelActorIntelligenceMP.m_Type = 8;
    gLevelActorIntelligenceMP.m_dwTypeFlags = 0;
    gLevelActorIntelligenceMP.m_nTexture = 0;
    gLevelActorIntelligenceMP.m_dwFlags |= 4;
    gLevelActorIntelligenceMP.m_Pickup.m_CollisionHeight = gLevelActorIntelligenceMP.m_Pickup.m_CollisionRadius = 122.88f;

    for (i = 0; i < 32; i++) {
        func_00227664(&pool->actors[i]);
    }
}
