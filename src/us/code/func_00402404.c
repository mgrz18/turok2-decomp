#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/scene.h"

void CAI__SetMode(void *a0, void *a1, s32 event);
void AI_PlayAnim(CGameObjectInstance *actor, void *ai, s32 state);

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
    for (e = gScene.m_InstancePool.m_ActiveList.m_pHead; e != NULL; e = e->m_pNext) {
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

extern u8 sMantidQueenSpotHitScript[];
extern u8 gEngineApp_m_OnScreen[];
void ScriptAnimator_SetScript(void *slot, void *arg);
void COnScreen__SetBossPercent(void *a0, s32 percent);

void MantidQueen_Hurt(void *a0, Obj00402404 *obj, Hit00402404 *hit) {
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
    s = &gMantidQueen;
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
        CAI__SetMode(a0, obj, 13);
    }
    s->spots[index].health -= hit->damage;
    ScriptAnimator_SetScript(&s->spots[index].anim, sMantidQueenSpotHitScript);
    if (s->spots[index].health <= 0) {
        s->spots[index].health = 0;
        obj->flags &= ~bit;
        if (obj->kind != 15) {
            CAI__SetMode(a0, obj, 13);
        }
        State_NextStep(s);
    }
    sum = 0;
    for (i = 0; i < 5; i++) {
        sum += s->spots[i].health;
    }
    percent = (sum >> 8) * 100 / 460;
    percent = MIN(MAX(percent, 0), 100);
    COnScreen__SetBossPercent(gEngineApp_m_OnScreen, percent);
}
