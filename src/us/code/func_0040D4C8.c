#include "common.h"
#include "structs/game_object.h"
#include "structs/script.h"

/* Second boss module: plays sound 0x284F, sends the module object to anim state 1 and steps the script to
 * the next phase. */


extern BlindOnePhase D_0043240C[];
extern void func_00275624(s32 sound);
extern void func_00243414(CGameObjectInstance *obj, void *ai, s32 state);

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
    if (D_0043240C[phase].init != NULL) {
        D_0043240C[phase].init(state);
    }
}

void func_0040D4C8(BlindOneState *state) {
    func_00275624(0x284F);
    func_00243414(state->boss, &state->boss->m_AI, 1);
    nextPhase(state);
}
