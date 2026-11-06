#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOnePhase D_0043240C[];

extern u8 D_00432494[];
extern u8 gScene[];
extern void CScene__MarkGeneratorsSaved(void *);
extern s32 CAI__SetMode(CGameObjectInstance *actor, void *ai, s32 state);

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

void BlindOne_PhaseGroup1Init(BlindOneState *m) {
    s32 i;

    for (i = 0; i < m->count1; i++) {
        CAI__SetMode(m->actors1[i], &m->actors1[i]->m_AI, 0);
    }
    StartTimer(m, D_00432494, m->script->seconds);
    CScene__MarkGeneratorsSaved(gScene);
}
