#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

void func_00243414(void *a0, void *a1, s32 event);
void func_002666B0(CGameObjectInstance *actor, void *ai, s32 state);
extern CListedInstance *D_8010FEB4;

/* func_00403520 */
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
        func_00243414(s->actor, ((u8 *)&s->actor->m_AI), 14);
    } else if (s->stepIndex == 8) {
        func_00243414(s->actor, ((u8 *)&s->actor->m_AI), 16);
    }
}

/* func_004035F8 */
static __inline__ s32 State_CountActive(WaveBossState *s) {
    s32 i;
    s32 n = 0;
    CListedInstance *e;

    for (i = 0; i < s->memberCount; i++) {
        if (s->members[i]->m_AI.m_Mode == 1) {
            n++;
        }
    }
    for (e = D_8010FEB4; e != NULL; e = e->m_pNext) {
        if (e->m_Instance.m_AI.m_Health > 0) {
            n++;
        }
    }
    return n;
}

/* func_00403430 */
static __inline__ void State_UpdateBoss(WaveBossState *s) {
    CGameObjectInstance *boss = s->boss;
    s32 aiState;

    boss->m_dwFlags &= ~0x10000000;
    aiState = boss->m_AI.m_VarBuffer[0];
    if (aiState == 0x5AB4) {
    } else if (aiState < 0x5AB5) {
    } else if (aiState == 0x5ABE) {
        if (boss->m_AI.m_AnimRepeatedCount != 0) {
            func_002666B0(s->boss, ((u8 *)&s->boss->m_AI), 0x5ADC);
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

extern u8 D_800F7078[];
extern WaveBossStep D_00430824[];
extern Part00401E30 D_004307B0[];
void func_002362E4(void *slot, void *actor);
s32 func_00225F6C(void *world, s32 key0, s32 key1, s32 key2, void *out, s32 max);
void *func_00225EBC(void *world, s32 a1, s32 id);

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

void func_00401E30(WaveBossState *s, CGameObjectInstance *actor) {
    s32 i;

    s->actor = actor;
    for (i = 0; i < 5; i++) {
        s->spots[i].health = 0x1900;
        func_002362E4(&s->spots[i].anim, actor);
    }
    s->spots[1].health = 0xFF00;
    s->spots[2].health = 0x9600;
    s->spots[3].health = 0x1E00;
    s->spots[4].health = 0x1900;
    s->toSpawn = 0;
    s->spots[0].health = 0;
    State_SetStep(s, D_00430824, 1);
    func_00225F6C(D_800F7078, 0x457, 9, 0x1D4D, &s->boss, 1);
    s->memberCount = func_00225F6C(D_800F7078, 0x651, 10, 0x1D4D, s->members, 8);
    for (i = 0; i < 2; i++) {
        s->intel[i] = func_00225EBC(D_800F7078, 1, D_004307B0[i].id);
    }
    s->blink = 0;
    s->blinkTimer = 0.0f;
}
