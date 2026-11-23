/* Second-module copy of BlindOne_PhaseGroup1Init (src/us/code/BlindOne_PhaseGroup1Init.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/scene.h"
#include "globals/boss.h"
#include "protos/scene.h"
#include "protos/ai.h"


static __inline__ void StartTimer(BlindOneState *m, void *proc, f32 seconds) {
    BlindOneMover *t;

    m->movers[0].seq = proc;
    t = &m->movers[0];
    t->pos = 0;
    t->timer = 0;
    t->step = 0;
    t->offset = 0;
    t->interval = seconds * 15.0f;
}

void BlindOne_PhaseGroup1Init_2(BlindOneState *m) {
    s32 i;

    for (i = 0; i < m->count1; i++) {
        CAI__SetMode(m->actors1[i], &m->actors1[i]->m_AI, 0);
    }
    StartTimer(m, sBlindOneMoverSeq1_2, m->script->seconds);
    CScene__MarkGeneratorsSaved(&gScene);
}
