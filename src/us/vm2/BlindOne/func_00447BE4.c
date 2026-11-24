#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

extern void BlindOne_EnterPhase_2(BlindOneState *m, s32 cmd);

void BlindOne_StartScript_2(BlindOneState *m, CGameObjectInstance *leader) {
    m->boss = leader;
    m->script = sBlindOneScript_2;
    BlindOne_EnterPhase_2(m, sBlindOneScript_2[0].id);
}
