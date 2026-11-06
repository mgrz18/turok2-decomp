/* Second-module copy of BlindOne_PhaseBossMode0Init (src/us/code/BlindOne_PhaseBossMode0Init.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOnePhase D_0043232C[];

extern u8 D_800F7078[];
extern void CScene__MarkGeneratorsSaved(void *);
extern s32 CAI__SetMode(CGameObjectInstance *actor, void *ai, s32 state);

static __inline__ void StartTimer(BlindOneState *m, s32 n, void *proc, f32 seconds) {
    BlindOneMover *t;

    m->movers[n].seq = proc;
    t = &m->movers[n];
    t->pos = 0;
    t->timer = 0;
    t->step = 0;
    t->offset = 0;
    t->interval = seconds * 15.0f;
}

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
    if (D_0043232C[cmd].init != NULL) {
        D_0043232C[cmd].init(m);
    }
}

void BlindOne_PhaseBossMode0Init_2(BlindOneState *m) {
    CAI__SetMode(m->boss, &m->boss->m_AI, 0);
    NextCommand(m);
}
