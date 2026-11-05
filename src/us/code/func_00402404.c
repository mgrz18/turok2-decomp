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

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 flags;
    /* 0x10 */ u8 pad1[0x34 - 0x10];
    /* 0x34 */ s8 kind;
} Obj00402404;

typedef struct {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ s32 mask;
} Source00402404;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 damage;
    /* 0x08 */ u8 pad1[0xC - 0x8];
    /* 0x0C */ s32 flags;
    /* 0x10 */ Source00402404 *source;
} Hit00402404;

extern u8 D_00430924[];
extern u8 D_801100F0[];
void func_00236314(void *slot, void *arg);
void func_0027AD88(void *a0, s32 percent);

void func_00402404(void *a0, Obj00402404 *obj, Hit00402404 *hit) {
    WaveBossState *s;
    WaveBossStep *step;
    s32 index;
    s32 bit;
    s32 sum;
    s32 i;
    s32 percent;

    if (hit->source == NULL) {
        return;
    }
    s = &D_8012F3A0;
    step = s->step;
    if (s->waveActive != 0) {
        return;
    }
    index = step->weakSpot;
    if (index == -1) {
        return;
    }
    if (index == 3 && obj->kind != 15) {
        return;
    }
    bit = 1 << index;
    if (!(hit->source->mask & bit)) {
        return;
    }
    if (obj->kind != 15 && (hit->flags & 0xC00000)) {
        func_00243414(a0, obj, 13);
    }
    s->spots[index].health -= hit->damage;
    func_00236314(&s->spots[index].anim, D_00430924);
    if (s->spots[index].health <= 0) {
        s->spots[index].health = 0;
        obj->flags &= ~bit;
        if (obj->kind != 15) {
            func_00243414(a0, obj, 13);
        }
        State_NextStep(s);
    }
    sum = 0;
    for (i = 0; i < 5; i++) {
        sum += s->spots[i].health;
    }
    percent = (sum >> 8) * 100 / 460;
    percent = MIN(MAX(percent, 0), 100);
    func_0027AD88(D_801100F0, percent);
}
