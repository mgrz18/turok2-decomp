#include "common.h"
#include "structs/game_object.h"

s32 func_00220408(s32 *, void *);               /* extern */
extern s32 D_800F7078;

void func_00262960(CGameObjectInstance *arg0) {
    if (func_00220408(&D_800F7078, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    }
}
