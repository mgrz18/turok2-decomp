#include "common.h"
#include "structs/event.h"

s32 func_002245BC(s32 *, u16, u16, s32); /* extern */
extern s32 D_800F7078;

void func_0026578C(LevelEvent *arg0) {
    func_002245BC(&D_800F7078, arg0->level, arg0->actor, 1);
}
