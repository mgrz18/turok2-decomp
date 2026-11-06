#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

extern void BlindOne_EnterPhase(BlindOneState *m, s32 cmd);

void BlindOne_StartScript(BlindOneState *m, CGameObjectInstance *leader) {
    m->boss = leader;
    m->script = sBlindOneScript;
    BlindOne_EnterPhase(m, sBlindOneScript[0].id);
}
