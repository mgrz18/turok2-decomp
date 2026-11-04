#include "common.h"
#include "structs/world.h"

s32 func_0042C700(s32, s32, s32);

void func_0042C4B8(CWorld *arg0, s32 arg1) {
    if (arg1 != arg0->m_nLevel) {
        arg0->m_bWarpSpawn = 0;
        func_0042C700((s32) arg0, ~arg1, 0);
    }
}
