/* Second-module copy of BlindOne_PhaseGroup2Init (src/us/code/BlindOne_PhaseGroup2Init.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/scene.h"
#include "globals/boss.h"

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

void BlindOne_PhaseGroup2Init_2(BlindOneState *m) {
    s32 i;

    for (i = 0; i < m->count2; i++) {
        CAI__SetMode(m->actors2[i], &m->actors2[i]->m_AI, 0);
    }
    StartTimer(m, 1, sBlindOneMoverSeq2_2, m->script->seconds);
    CScene__MarkGeneratorsSaved(&gScene);
}
