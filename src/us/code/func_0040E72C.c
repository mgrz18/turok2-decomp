#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/scene.h"
#include "globals/boss.h"
#include "protos/scene.h"
#include "protos/ai.h"


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

void BlindOne_PhaseGroup2Init(BlindOneState *m) {
    s32 i;

    for (i = 0; i < m->count2; i++) {
        CAI__SetMode(m->actors2[i], &m->actors2[i]->m_AI, 0);
    }
    StartTimer(m, 1, sBlindOneMoverSeq2, m->script->seconds);
    CScene__MarkGeneratorsSaved(&gScene);
}
