#include "common.h"

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

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 pad1[0x140 - 0xD8];
    /* 0x140 */ u8 ai[0x174 - 0x140];
    /* 0x174 */ s8 state;
    /* 0x175 */ u8 pad2[0x207 - 0x175];
    /* 0x207 */ s8 active;
    /* 0x208 */ u8 pad3[0x24C - 0x208];
    /* 0x24C */ s32 aiState;
} Actor;

typedef struct Enemy {
    /* 0x000 */ u8 pad0[0x144];
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad1[0x28C - 0x148];
    /* 0x28C */ struct Enemy *next;
} Enemy;

typedef struct {
    /* 0x000 */ Actor *actor;
    /* 0x004 */ u8 pad0[0x8 - 0x4];
    /* 0x008 */ Slot slots[5];
    /* 0x210 */ s32 stepIndex;
    /* 0x214 */ Step *step;
    /* 0x218 */ s32 total;
    /* 0x21C */ s32 timer;
    /* 0x220 */ Actor *boss;
    /* 0x224 */ Actor *members[8];
    /* 0x244 */ s32 count;
    /* 0x248 */ void *handles[2];
    /* 0x250 */ s32 enabled;
    /* 0x254 */ s32 blink;
    /* 0x258 */ f32 blinkTimer;
} State;

void func_00243414(void *a0, void *a1, s32 event);
void func_002666B0(Actor *actor, void *ai, s32 state);
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
        func_00243414(s->actor, s->actor->ai, 14);
    } else if (s->stepIndex == 8) {
        func_00243414(s->actor, s->actor->ai, 16);
    }
}

/* func_004035F8 */
static __inline__ s32 State_CountActive(State *s) {
    s32 i;
    s32 n = 0;
    Enemy *e;

    for (i = 0; i < s->count; i++) {
        if (s->members[i]->state == 1) {
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
    Actor *boss = s->boss;
    s32 aiState;

    boss->flags &= ~0x10000000;
    aiState = boss->aiState;
    if (aiState == 0x5AB4) {
    } else if (aiState < 0x5AB5) {
    } else if (aiState == 0x5ABE) {
        if (boss->active != 0) {
            func_002666B0(s->boss, s->boss->ai, 0x5ADC);
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
extern Step D_00430824[];
extern Part00401E30 D_004307B0[];
void func_002362E4(void *slot, void *actor);
s32 func_00225F6C(void *world, s32 key0, s32 key1, s32 key2, void *out, s32 max);
void *func_00225EBC(void *world, s32 a1, s32 id);

static __inline__ void State_SetStep(State *s, Step *step, s32 index) {
    s->step = step;
    s->stepIndex = index;
    s->total += step->value;
    s->timer = 0;
    if (step->active != 0 && step->value != 0) {
        s->enabled = 1;
    } else {
        s->enabled = 0;
    }
}

void func_00401E30(State *s, Actor *actor) {
    s32 i;

    s->actor = actor;
    for (i = 0; i < 5; i++) {
        s->slots[i].value = 0x1900;
        func_002362E4(s->slots[i].body, actor);
    }
    s->slots[1].value = 0xFF00;
    s->slots[2].value = 0x9600;
    s->slots[3].value = 0x1E00;
    s->slots[4].value = 0x1900;
    s->total = 0;
    s->slots[0].value = 0;
    State_SetStep(s, D_00430824, 1);
    func_00225F6C(D_800F7078, 0x457, 9, 0x1D4D, &s->boss, 1);
    s->count = func_00225F6C(D_800F7078, 0x651, 10, 0x1D4D, s->members, 8);
    for (i = 0; i < 2; i++) {
        s->handles[i] = func_00225EBC(D_800F7078, 1, D_004307B0[i].id);
    }
    s->blink = 0;
    s->blinkTimer = 0.0f;
}
