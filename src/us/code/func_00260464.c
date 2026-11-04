#include "common.h"
#include "structs/game_object.h"

s32 func_00245BAC(s32, void *, f32, f32);       /* extern */
f32 func_00246310(s32, CVector3);               /* angle from the actor heading to a point */

extern f32 D_800A798C;
extern f32 D_800A7990;
extern CVector3 D_800B06A0;

void func_00260464(s32 arg0, CAI *arg1) {
    f32 var_f20;

    var_f20 = D_800A798C;
    if ((u32) ((u8) arg1->m_ModeBefore - 1) < 2U) {
        var_f20 = D_800A7990;
    }
    func_00245BAC(arg0, arg1, var_f20, func_00246310(arg0, D_800B06A0));
}
