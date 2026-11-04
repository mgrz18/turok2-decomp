#include "common.h"
#include "structs/game_object.h"
#include "structs/script.h"

extern BlindOnePhase D_0043240C[];

void func_0040E648(BlindOneState *m) {
    BlindOneStep *script;
    s32 cmd;
    s32 i;

    if (m->alive1 == 0) {
        script = m->script;
        m->script = script + 1;
        cmd = script[1].id;
        m->movers[0].seq = NULL;
        m->movers[1].seq = NULL;
        m->movers[2].seq = NULL;
        for (i = 0; i < m->count1; i++) {
            m->actors1[i]->m_AI.m_VarBuffer[0] = 0;
        }
        for (i = 0; i < m->count2; i++) {
            m->actors2[i]->m_AI.m_VarBuffer[0] = 0;
        }
        for (i = 0; i < m->count3; i++) {
            m->actors3[i]->m_AI.m_VarBuffer[0] = 0;
        }
        m->phase = cmd;
        if (D_0043240C[cmd].init != NULL) {
            D_0043240C[cmd].init(m);
        }
    }
}
