/* Second-module copy of func_0040EC14 (src/us/code/func_0040EC14.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOnePhase D_0043232C[];


/* Starts script command `cmd`: stops the movers, clears every member's flag and runs the command. */
void func_00447C14(BlindOneState *m, s32 cmd) {
    s32 i;

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
    if (D_0043232C[cmd].init != NULL) {
        D_0043232C[cmd].init(m);
    }
}
