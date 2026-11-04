#include "common.h"
#include "structs/game_object.h"

s32 func_00220408(void *, void *);                  /* extern */
void func_00243414(void *, CAI *, s32);             /* extern */
extern u8 D_800F7078[];

void func_00265A80(CGameObjectInstance *arg0, CAI *arg1) {
    if (func_00220408(D_800F7078, arg0) == 1) {
        func_00243414(arg0, arg1, 1);
        return;
    }
    func_00243414(arg0, arg1, 0);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x10000);
}
