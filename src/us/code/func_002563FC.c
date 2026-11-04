#include "common.h"
#include "structs/player.h"

s32 func_0024E5F0(void *, s32, s32);    /* extern */
s32 func_00251A2C();                            /* extern */

void func_002563FC(CPlayer *arg0, s32 arg1) {
    func_00251A2C();
    if (arg0->m_Speed != 0.0f) {
        func_0024E5F0(arg0, arg1, 0x22);
    }
}
