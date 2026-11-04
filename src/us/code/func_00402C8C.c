#include "common.h"
#include "structs/game_object.h"

s32 func_00225C84(s32);

extern s32 D_800F7078;

void func_00402C8C(CGameObjectInstance *arg0) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x2000);
    func_00225C84((s32) &D_800F7078);
}
