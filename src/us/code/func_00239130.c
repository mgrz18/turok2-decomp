#include "common.h"
#include "structs/game_object.h"

void func_00239130(CGameObjectInstance *arg0, s32 arg1, s32 arg2, CVector3 arg3, s32 arg4) {
    if (arg0->header.m_Type == 1) {
        arg0->m_dwHotPointFlags |= 1 << arg4;
    }
}
