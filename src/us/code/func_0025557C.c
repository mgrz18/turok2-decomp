#include "common.h"
#include "structs/player.h"

s32 func_0024E5F0(s32, s32, s32);

void func_0025557C(CPlayer *arg0, s32 arg1) {
    if ((arg0->m_Speed != 0.0f) || (arg0->m_SideSpeed != 0.0f)) {
        func_0024E5F0((s32) arg0, arg1, 3);
    }
}
