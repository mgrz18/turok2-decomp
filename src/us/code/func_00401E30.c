#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

void CAI__SetMode(void *a0, void *a1, s32 event);
void AI_PlayAnim(CGameObjectInstance *actor, void *ai, s32 state);
extern CListedInstance *gScene_m_InstancePool_m_ActiveList;

/* MantidQueen_NextStep */
static __inline__ void State_NextStep(WaveBossState *s) {
    WaveBossStep *step;
    s32 value;

    s->step++;
    do {
        s->stepIndex++;
        step = s->step;
        value = step->spawnCount;
        s->timer = 0;
        s->toSpawn += value;
    } while (0);
    if (step->partPicks != 0 && step->spawnCount != 0) {
        s->waveActive = 1;
    } else {
        s->waveActive = 0;
    }
    if (s->stepIndex == 7) {
        CAI__SetMode(s->actor, ((u8 *)&s->actor->m_AI), 14);
    } else if (s->stepIndex == 8) {
        CAI__SetMode(s->actor, ((u8 *)&s->actor->m_AI), 16);
    }
}

/* MantidQueen_CountActiveMinions */
static __inline__ s32 State_CountActive(WaveBossState *s) {
    s32 i;
    s32 n = 0;
    CListedInstance *e;

    for (i = 0; i < s->memberCount; i++) {
        if (s->members[i]->m_AI.m_Mode == 1) {
            n++;
        }
    }
    for (e = gScene_m_InstancePool_m_ActiveList; e != NULL; e = e->m_pNext) {
        if (e->m_Instance.m_AI.m_Health > 0) {
            n++;
        }
    }
    return n;
}

/* MantidQueen_UpdateBossAnim */
static __inline__ void State_UpdateBoss(WaveBossState *s) {
    CGameObjectInstance *boss = s->boss;
    s32 aiState;

    boss->m_dwFlags &= ~0x10000000;
    aiState = boss->m_AI.m_VarBuffer[0];
    if (aiState == 0x5AB4) {
    } else if (aiState < 0x5AB5) {
    } else if (aiState == 0x5ABE) {
        if (boss->m_AI.m_AnimRepeatedCount != 0) {
            AI_PlayAnim(s->boss, ((u8 *)&s->boss->m_AI), 0x5ADC);
        }
    }
}

typedef struct {
    f32 x, y, z;
} Vec3_00401E30;

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 model;
    /* 0x08 */ Vec3_00401E30 offset;
} Part00401E30;

extern u8 gScene[];
void ScriptAnimator_Start(void *slot, void *actor);
s32 CScene__FindInstances(void *world, s32 key0, s32 key1, s32 key2, void *out, s32 max);
void *CScene__FindIntelligence(void *world, s32 a1, s32 id);

static __inline__ void State_SetStep(WaveBossState *s, WaveBossStep *step, s32 index) {
    s->step = step;
    s->stepIndex = index;
    s->toSpawn += step->spawnCount;
    s->timer = 0;
    if (step->partPicks != 0 && step->spawnCount != 0) {
        s->waveActive = 1;
    } else {
        s->waveActive = 0;
    }
}

void MantidQueen_Init(WaveBossState *s, CGameObjectInstance *actor) {
    s32 i;

    s->actor = actor;
    for (i = 0; i < 5; i++) {
        s->spots[i].health = 0x1900;
        ScriptAnimator_Start(&s->spots[i].anim, actor);
    }
    s->spots[1].health = 0xFF00;
    s->spots[2].health = 0x9600;
    s->spots[3].health = 0x1E00;
    s->spots[4].health = 0x1900;
    s->toSpawn = 0;
    s->spots[0].health = 0;
    State_SetStep(s, sMantidQueenScript, 1);
    CScene__FindInstances(gScene, 0x457, 9, 0x1D4D, &s->boss, 1);
    s->memberCount = CScene__FindInstances(gScene, 0x651, 10, 0x1D4D, s->members, 8);
    for (i = 0; i < 2; i++) {
        s->intel[i] = CScene__FindIntelligence(gScene, 1, sMantidQueenMinions[i].id);
    }
    s->blink = 0;
    s->blinkTimer = 0.0f;
}
