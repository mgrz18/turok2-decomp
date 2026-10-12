#include "common.h"
#include "m2c_macros.h"
#include "structs/player.h"

void func_00256878(CPlayer *arg0, void *arg1, s32 arg2, s32 *arg3) {
    if (M2C_FIELD(arg1, s32 *, 0xBF4) != arg0->m_bOxygenOut) {
        *arg3 += 1;
    }
}
