/* Second-module copy of BlindOne_StartScript (src/us/code/BlindOne_StartScript.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOnePhase sBlindOnePhases_2[];
extern BlindOneStep sBlindOneScript_2[]; /* the script */
extern void BlindOne_EnterPhase_2(BlindOneState *m, s32 cmd);

void BlindOne_StartScript_2(BlindOneState *m, CGameObjectInstance *leader) {
    m->boss = leader;
    m->script = sBlindOneScript_2;
    BlindOne_EnterPhase_2(m, sBlindOneScript_2[0].id);
}
