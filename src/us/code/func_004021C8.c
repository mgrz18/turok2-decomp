#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ s32 active;
    /* 0x04 */ s32 value;
    /* 0x08 */ u8 pad0[0x14 - 0x8];
    /* 0x14 */ s32 slot;
    /* 0x18 */ f32 onTime;
    /* 0x1C */ f32 offTime;
} Step;

typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ u8 body[0x68 - 0x4];
} Slot;


typedef struct Enemy {
    /* 0x000 */ u8 pad0[0x144];
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad1[0x28C - 0x148];
    /* 0x28C */ struct Enemy *next;
} Enemy;

typedef struct {
    /* 0x000 */ CGameObjectInstance *actor;
    /* 0x004 */ u8 pad0[0x8 - 0x4];
    /* 0x008 */ Slot slots[5];
    /* 0x210 */ s32 stepIndex;
    /* 0x214 */ Step *step;
    /* 0x218 */ s32 total;
    /* 0x21C */ s32 timer;
    /* 0x220 */ CGameObjectInstance *boss;
    /* 0x224 */ CGameObjectInstance *members[8];
    /* 0x244 */ s32 count;
    /* 0x248 */ void *handles[2];
    /* 0x250 */ s32 enabled;
    /* 0x254 */ s32 blink;
    /* 0x258 */ f32 blinkTimer;
} State;

void func_00243414(void *a0, void *a1, s32 event);
void func_002666B0(CGameObjectInstance *actor, void *ai, s32 state);
extern Enemy *D_8010FEB4;

/* func_00403520 */
static __inline__ void State_NextStep(State *s) {
    Step *step;
    s32 value;

    s->step++;
    do {
        s->stepIndex++;
        step = s->step;
        value = step->value;
        s->timer = 0;
        s->total += value;
    } while (0);
    if (step->active != 0 && step->value != 0) {
        s->enabled = 1;
    } else {
        s->enabled = 0;
    }
    if (s->stepIndex == 7) {
        func_00243414(s->actor, ((u8 *)&s->actor->m_AI), 14);
    } else if (s->stepIndex == 8) {
        func_00243414(s->actor, ((u8 *)&s->actor->m_AI), 16);
    }
}

/* func_004035F8 */
static __inline__ s32 State_CountActive(State *s) {
    s32 i;
    s32 n = 0;
    Enemy *e;

    for (i = 0; i < s->count; i++) {
        if (s->members[i]->m_AI.m_Mode == 1) {
            n++;
        }
    }
    for (e = D_8010FEB4; e != NULL; e = e->next) {
        if (e->health > 0) {
            n++;
        }
    }
    return n;
}

/* func_00403430 */
static __inline__ void State_UpdateBoss(State *s) {
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
void func_00401FCC(State *s, s32 *active);

void func_004021C8(State *s) {
    CGameObjectInstance *actor = s->actor;
    s32 active;
    s32 i;

    for (i = 0; i < 5; i++) {
        func_00235510(s->slots[i].body);
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    if (actor->m_AI.m_Mode < 2) {
        return;
    }
    active = State_CountActive(s);
    if (s->total != 0) {
        func_00401FCC(s, &active);
    }
    if (s->enabled != 0 && s->total + active <= 0) {
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
