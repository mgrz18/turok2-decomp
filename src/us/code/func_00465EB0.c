#include "common.h"
#include "structs/level_actor.h"

/* Reset the world's pool of run-time actors (all on the free list) and the two
 * pickup effect emitters. */

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ u32 flags;
    /* 0x08 */ u8 pad8[0xA];
    /* 0x12 */ u8 count;
    /* 0x13 */ u8 pad13;
    /* 0x14 */ s32 active;
    /* 0x18 */ f32 width;
    /* 0x1C */ f32 height;
} Emitter;

extern Emitter gLevelActorIntelligence;
extern Emitter gLevelActorIntelligenceMP;

void func_0027BD10(Emitter *emitter);
void func_00227664(CPooledLevelActor *actor);

void SceneActorPool_Construct_2(CLevelActorPool *pool) {
    s32 i;

    pool->m_pFree = pool->m_Actors;
    pool->m_pTail = NULL;
    pool->m_pHead = NULL;
    for (i = 0; i < 31; i++) {
        pool->m_Actors[i].m_pNext = &pool->m_Actors[i + 1];
        pool->m_Actors[i].m_pPrev = &pool->m_Actors[i - 1];
    }
    pool->m_Actors[0].m_pPrev = NULL;
    pool->m_Actors[31].m_pNext = NULL;

    func_0027BD10(&gLevelActorIntelligence);
    gLevelActorIntelligence.kind = 8;
    gLevelActorIntelligence.active = 1;
    gLevelActorIntelligence.count = 0;
    gLevelActorIntelligence.flags |= 4;
    func_0027BD10(&gLevelActorIntelligenceMP);
    gLevelActorIntelligenceMP.kind = 8;
    gLevelActorIntelligenceMP.active = 0;
    gLevelActorIntelligenceMP.count = 0;
    gLevelActorIntelligenceMP.flags |= 4;
    gLevelActorIntelligenceMP.width = 122.88f;
    gLevelActorIntelligenceMP.height = 122.88f;

    for (i = 0; i < 32; i++) {
        func_00227664(&pool->m_Actors[i]);
    }
}
