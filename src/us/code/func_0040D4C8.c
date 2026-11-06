#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* Second boss module: plays sound 0x284F, sends the module object to anim state 1 and steps the script to
 * the next phase. */


extern void Sound__Play2D(s32 sound);
extern void CAI__SetMode(CGameObjectInstance *obj, void *ai, s32 state);

/* Steps the script: clears the movers and the actors' m_BlindOne.m_nPhaseHits, enters the next phase and runs its init. */
static __inline__ void nextPhase(BlindOneState *state) {
    s32 phase;
    s32 i;

    phase = (++state->script)->id;
    state->movers[0].seq = 0;
    state->movers[1].seq = 0;
    state->movers[2].seq = 0;
    for (i = 0; i < state->count1; i++) {
        state->actors1[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < state->count2; i++) {
        state->actors2[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < state->count3; i++) {
        state->actors3[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    state->phase = phase;
    if (sBlindOnePhases[phase].init != NULL) {
        sBlindOnePhases[phase].init(state);
    }
}

void BlindOne_PhaseBossMode1Init(BlindOneState *state) {
    Sound__Play2D(0x284F);
    CAI__SetMode(state->boss, &state->boss->m_AI, 1);
    nextPhase(state);
}
