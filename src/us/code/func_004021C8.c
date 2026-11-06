#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/engine.h"

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

void Animator_Update(void *slot);
s32 Cinema_PathPlaying(void);
void MantidQueen_SendMinions(WaveBossState *s, s32 *active);

void MantidQueen_Update(WaveBossState *s) {
    CGameObjectInstance *actor = s->actor;
    s32 active;
    s32 i;

    for (i = 0; i < 5; i++) {
        Animator_Update(&s->spots[i].anim);
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    if (actor->m_AI.m_Mode < 2) {
        return;
    }
    active = State_CountActive(s);
    if (s->toSpawn != 0) {
        MantidQueen_SendMinions(s, &active);
    }
    if (s->waveActive != 0 && s->toSpawn + active <= 0) {
        State_NextStep(s);
    }
    State_UpdateBoss(s);
    s->blinkTimer -= gFrameIncrement;
    if (s->blinkTimer <= 0.0f) {
        s->blink ^= 1;
        if (s->blink) {
            s->blinkTimer = s->step->onTime;
        } else {
            s->blinkTimer = s->step->offTime;
        }
    }
}
