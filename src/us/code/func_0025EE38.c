#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

s32 func_00220408(M2C_UNK *, void *);               /* extern */
s32 Cinema_PathPlaying();                                /* extern */
extern M2C_UNK D_800F7078;

void func_0025EE38(CGameObjectInstance *arg0, s32 *arg1) {
    if (func_00220408(&D_800F7078, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    }
    if (Cinema_PathPlaying() != 0) {
        *arg1 |= 0x200;
    }
}
