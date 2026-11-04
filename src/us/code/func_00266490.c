#include "common.h"
#include "structs/game_object.h"

s32 func_00266644(s32, s32);

void func_00266490(CGameObjectInstance *obj, CAI *ai) {
    ai->m_pfDraw = func_00266644;
}
