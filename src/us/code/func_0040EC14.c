#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOnePhase sBlindOnePhases[];


/* Starts script command `cmd`: stops the movers, clears every member's flag and runs the command. */
void BlindOne_EnterPhase(BlindOneState *m, s32 cmd) {
    s32 i;

    m->movers[0].seq = NULL;
    m->movers[1].seq = NULL;
    m->movers[2].seq = NULL;
    for (i = 0; i < m->count1; i++) {
        m->actors1[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < m->count2; i++) {
        m->actors2[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < m->count3; i++) {
        m->actors3[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    m->phase = cmd;
    if (sBlindOnePhases[cmd].init != NULL) {
        sBlindOnePhases[cmd].init(m);
    }
}
