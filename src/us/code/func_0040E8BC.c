#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOnePhase D_0043240C[];

extern u8 D_004324F4[];

extern u8 gScene[];
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

void BlindOne_PhaseGroup3Init(BlindOneState *m) {
    CAI__SetMode(m->actors3[0], &m->actors3[0]->m_AI, 0);
    StartTimer(m, 2, D_004324F4, m->script->seconds);
    CScene__MarkGeneratorsSaved(gScene);
}
