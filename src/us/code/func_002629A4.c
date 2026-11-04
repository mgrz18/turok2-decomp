#include "common.h"
#include "structs/game_object.h"

s32 func_00220408(s32 *, s32);                  /* extern */
s32 func_00243414(s32, void *, s32);        /* extern */
s32 func_00262960(s32, void *, s32);        /* extern */
extern s32 D_800F7078;

void func_002629A4(s32 arg0, CAI *ai, s32 arg2) {
    if ((func_00220408(&D_800F7078, arg0) == 0) && (ai->m_Mode == 0)) {
        func_00262960(arg0, ai, arg2);
        func_00243414(arg0, ai, 1);
    }
}
