#include "common.h"
#include "structs/event.h"

void func_002245BC(void *, u16, u16, s32);          /* extern */
extern u8 D_800F7078[];

void func_002637C4(LevelEvent *arg0) {
    func_002245BC(D_800F7078, arg0->level, arg0->actor, 0);
}
