#include "common.h"
#include "structs/game_object.h"

typedef struct { s32 x, y, z; } V3;

void func_00239130(CGameObjectInstance *arg0, s32 arg1, s32 arg2, V3 arg3, s32 arg4) {
    if (arg0->header.m_Type == 1) {
        arg0->m_dwHotPointFlags |= 1 << arg4;
    }
}
