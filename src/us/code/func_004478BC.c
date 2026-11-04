/* Second-module copy of func_0040E8BC (src/us/code/func_0040E8BC.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOnePhase D_0043232C[];

extern u8 D_00432414[];

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

void func_004478BC(BlindOneState *m) {
    func_00243414(m->actors3[0], &m->actors3[0]->m_AI, 0);
    StartTimer(m, 2, D_00432414, m->script->seconds);
    func_00225C84(D_800F7078);
}
