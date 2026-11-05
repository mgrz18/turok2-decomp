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

extern f32 D_800B6D28;
void func_00235510(void *slot);
s32 Cinema_PathPlaying(void);
void func_00401FCC(WaveBossState *s, s32 *active);

void func_004021C8(WaveBossState *s) {
    CGameObjectInstance *actor = s->actor;
    s32 active;
    s32 i;

    for (i = 0; i < 5; i++) {
        func_00235510(&s->spots[i].anim);
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    if (actor->m_AI.m_Mode < 2) {
        return;
    }
    active = State_CountActive(s);
    if (s->toSpawn != 0) {
        func_00401FCC(s, &active);
    }
    if (s->waveActive != 0 && s->toSpawn + active <= 0) {
        State_NextStep(s);
    }
    State_UpdateBoss(s);
    s->blinkTimer -= D_800B6D28;
    if (s->blinkTimer <= 0.0f) {
        s->blink ^= 1;
        if (s->blink) {
            s->blinkTimer = s->step->onTime;
        } else {
            s->blinkTimer = s->step->offTime;
        }
    }
}
