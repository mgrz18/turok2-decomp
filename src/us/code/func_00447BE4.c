/* Second-module copy of BlindOne_StartScript (src/us/code/BlindOne_StartScript.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOnePhase D_0043232C[];
extern BlindOneStep D_00432260[]; /* the script */
extern void BlindOne_EnterPhase(BlindOneState *m, s32 cmd);

void BlindOne_StartScript_2(BlindOneState *m, CGameObjectInstance *leader) {
    m->boss = leader;
    m->script = D_00432260;
    BlindOne_EnterPhase(m, D_00432260[0].id);
}
