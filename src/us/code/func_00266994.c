#include "common.h"
#include "structs/game_object.h"

s32 func_0021A7EC(void *);                      /* extern */
s32 func_00220260(s32 *, void *, s32);  /* extern */
s32 Cinema_PathPlaying();                                /* extern */
extern s32 D_800F7078;

void func_00266994(CGameObjectInstance *arg0, s32 *arg1) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    if (Cinema_PathPlaying() != 0) {
        *arg1 |= 0x200;
    }
    func_0021A7EC(arg0);
    func_00220260(&D_800F7078, arg0, 0);
}
