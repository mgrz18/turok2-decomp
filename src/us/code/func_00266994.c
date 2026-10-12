#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

M2C_UNK func_0021A7EC(void *);                      /* extern */
M2C_UNK func_00220260(M2C_UNK *, void *, M2C_UNK);  /* extern */
s32 Cinema_PathPlaying();                                /* extern */
extern M2C_UNK D_800F7078;

void func_00266994(CGameObjectInstance *arg0, s32 *arg1) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    if (Cinema_PathPlaying() != 0) {
        *arg1 |= 0x200;
    }
    func_0021A7EC(arg0);
    func_00220260(&D_800F7078, arg0, 0);
}
