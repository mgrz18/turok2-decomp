/* Second-module copy of func_0040EBE4 (src/us/code/func_0040EBE4.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOnePhase D_0043232C[];
extern BlindOneStep D_00432260[]; /* the script */
extern void func_0040EC14(BlindOneState *m, s32 cmd);

void func_00447BE4(BlindOneState *m, CGameObjectInstance *leader) {
    m->boss = leader;
    m->script = D_00432260;
    func_0040EC14(m, D_00432260[0].id);
}
