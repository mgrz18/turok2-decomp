#include "common.h"
#include "structs/game_object.h"
#include "structs/script.h"

extern BlindOnePhase D_0043240C[];

extern u8 D_004324C8[];

extern u8 D_800F7078[];
extern void func_00225C84(void *);
extern s32 func_00243414(CGameObjectInstance *actor, void *ai, s32 state);

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

void func_0040E72C(BlindOneState *m) {
    s32 i;

    for (i = 0; i < m->count2; i++) {
        func_00243414(m->actors2[i], &m->actors2[i]->m_AI, 0);
    }
    StartTimer(m, 1, D_004324C8, m->script->seconds);
    func_00225C84(D_800F7078);
}
