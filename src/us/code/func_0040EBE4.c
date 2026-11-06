#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOnePhase D_0043240C[];
extern BlindOneStep D_00432340[]; /* the script */
extern void BlindOne_EnterPhase(BlindOneState *m, s32 cmd);

void BlindOne_StartScript(BlindOneState *m, CGameObjectInstance *leader) {
    m->boss = leader;
    m->script = D_00432340;
    BlindOne_EnterPhase(m, D_00432340[0].id);
}
