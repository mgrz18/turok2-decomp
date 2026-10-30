#include "common.h"

typedef struct {
    /* 0x00 */ s32 active;
    /* 0x04 */ s32 value;
    /* 0x08 */ u8 pad0[0x20 - 0x8];
} Step00403520;

typedef struct {
    /* 0x000 */ u8 *actor;
    /* 0x004 */ u8 pad0[0x210 - 0x4];
    /* 0x210 */ s32 stepIndex;
    /* 0x214 */ Step00403520 *step;
    /* 0x218 */ s32 total;
    /* 0x21C */ s32 timer;
    /* 0x220 */ u8 pad1[0x250 - 0x220];
    /* 0x250 */ s32 enabled;
} State00403520;

void func_00243414(u8 *actor, u8 *ai, s32 a2);

/* Moves the encounter to its next step: accumulates the step's value, sets
 * whether it is enabled, and fires the actor's events for steps 7 and 8. */
void func_00403520(State00403520 *s) {
    Step00403520 *step;
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
        func_00243414(s->actor, s->actor + 0x140, 14);
    } else if (s->stepIndex == 8) {
        func_00243414(s->actor, s->actor + 0x140, 16);
    }
}
