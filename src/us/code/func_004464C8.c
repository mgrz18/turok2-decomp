/* Second-module copy of func_0040D4C8 (src/us/code/func_0040D4C8.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Second boss module: plays sound 0x284F, sends the module object to anim state 1 and steps the script to
 * the next phase. */

struct BlindOneState;

typedef struct {
    /* 0x00 */ void (*init)(struct BlindOneState *state);
    /* 0x04 */ void (*update)(struct BlindOneState *state);
} PhaseFuncs;

typedef struct Obj2 {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj2;

extern PhaseFuncs D_0043232C[];
extern void func_00275624(s32 sound);
extern void func_00243414(Obj2 *obj, void *ai, s32 state);

/* Steps the script: clears the movers and the actors' 0x24C, enters the next phase and runs its init. */
static __inline__ void nextPhase(BlindOneState *state) {
    s32 phase;
    s32 i;

    phase = (++state->script)->id;
    state->movers[0].seq = 0;
    state->movers[1].seq = 0;
    state->movers[2].seq = 0;
    for (i = 0; i < state->count1; i++) {
        state->actors1[i]->m_AI.m_VarBuffer[0] = 0;
    }
    for (i = 0; i < state->count2; i++) {
        state->actors2[i]->m_AI.m_VarBuffer[0] = 0;
    }
    for (i = 0; i < state->count3; i++) {
        state->actors3[i]->m_AI.m_VarBuffer[0] = 0;
    }
    state->phase = phase;
    if (D_0043232C[phase].init != NULL) {
        D_0043232C[phase].init(state);
    }
}

void func_004464C8(BlindOneState *state) {
    func_00275624(0x284F);
    func_00243414((Obj2 *)state->obj, ((Obj2 *)state->obj)->ai, 1);
    nextPhase(state);
}
