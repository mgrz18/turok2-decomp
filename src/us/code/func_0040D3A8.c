#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Second boss module: once none of its third group of actors has flag 0x100 left, steps the script to the
 * next phase. */


extern BlindOnePhase D_0043240C[];

/* Steps the script: clears the movers and the actors' m_BlindOne.m_PhaseState, enters the next phase and runs its init. */
static __inline__ void nextPhase(BlindOneState *state) {
    s32 phase;
    s32 i;

    phase = (++state->script)->id;
    state->movers[0].seq = 0;
    state->movers[1].seq = 0;
    state->movers[2].seq = 0;
    for (i = 0; i < state->count1; i++) {
        state->actors1[i]->m_AI.m_BlindOne.m_PhaseState = 0;
    }
    for (i = 0; i < state->count2; i++) {
        state->actors2[i]->m_AI.m_BlindOne.m_PhaseState = 0;
    }
    for (i = 0; i < state->count3; i++) {
        state->actors3[i]->m_AI.m_BlindOne.m_PhaseState = 0;
    }
    state->phase = phase;
    if (D_0043240C[phase].init != NULL) {
        D_0043240C[phase].init(state);
    }
}

void func_0040D3A8(BlindOneState *state) {
    s32 done = 1;
    s32 i;

    for (i = 0; i < state->count3; i++) {
        if (state->actors3[i]->m_dwFlags & 0x100) {
            done = 0;
            break;
        }
    }
    if (done) {
        nextPhase(state);
    }
}
