#include "common.h"
#include "structs/game_object.h"

s32 func_00220260(s32 *, void *, s32);  /* extern */
extern s32 D_800F7078;

void func_0026484C(CGameObjectInstance *arg0) {
    func_00220260(&D_800F7078, arg0, 1);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x100);
}
