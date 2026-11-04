#include "common.h"
#include "structs/game_object.h"
#include "structs/script.h"

extern BlindOnePhase D_0043240C[];
extern BlindOneStep D_00432340[]; /* the script */
extern void func_0040EC14(BlindOneState *m, s32 cmd);

void func_0040EBE4(BlindOneState *m, CGameObjectInstance *leader) {
    m->boss = leader;
    m->script = D_00432340;
    func_0040EC14(m, D_00432340[0].id);
}
