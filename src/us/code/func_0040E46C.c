#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOnePhase D_0043240C[];

extern s32 func_00284204(BlindOneState *m);

void func_0040E46C(BlindOneState *m) {
    BlindOneStep *script;
    s32 cmd;
    s32 i;

    if (func_00284204(m)) {
        script = m->script;
        m->script = script + 1;
        cmd = script[1].id;
        m->movers[0].seq = NULL;
        m->movers[1].seq = NULL;
        m->movers[2].seq = NULL;
        for (i = 0; i < m->count1; i++) {
            m->actors1[i]->m_AI.m_BlindOne.m_PhaseState = 0;
        }
        for (i = 0; i < m->count2; i++) {
            m->actors2[i]->m_AI.m_BlindOne.m_PhaseState = 0;
        }
        for (i = 0; i < m->count3; i++) {
            m->actors3[i]->m_AI.m_BlindOne.m_PhaseState = 0;
        }
        m->phase = cmd;
        if (D_0043240C[cmd].init != NULL) {
            D_0043240C[cmd].init(m);
        }
    }
}
