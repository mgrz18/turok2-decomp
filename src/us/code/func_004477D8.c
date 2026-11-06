/* Second-module copy of BlindOne_PhaseGroup2Update (src/us/code/BlindOne_PhaseGroup2Update.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOnePhase sBlindOnePhases_2[];

/* Advances the script by one command, stops the movers and runs the new command. */
static __inline__ void NextCommand(BlindOneState *m) {
    BlindOneStep *script;
    s32 cmd;
    s32 i;

    script = m->script;
    m->script = script + 1;
    cmd = script[1].id;
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
    if (sBlindOnePhases_2[cmd].init != NULL) {
        sBlindOnePhases_2[cmd].init(m);
    }
}

void BlindOne_PhaseGroup2Update_2(BlindOneState *m) {
    if (m->alive2 == 0) {
        NextCommand(m);
    }
}
