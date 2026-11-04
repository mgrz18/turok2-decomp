#include "common.h"
#include "structs/game_object.h"

f32 func_0021170C(f32, f32);
s32 func_0025E428(s32, s32);

extern f32 D_800A79D0;
extern f32 D_800B6D28;

void func_00260990(s32 arg0, CAI *ai) {
    f32 temp_f0;
    f32 temp_f20;

    if ((func_0025E428(arg0, (s32) ai) != 1) || (ai->m_Mode != 4)) {
        temp_f20 = D_800A79D0;
        *(f32 *) &ai->m_VarBuffer[1] = func_0021170C(*(f32 *) &ai->m_VarBuffer[1], temp_f20);
        *(f32 *) &ai->m_VarBuffer[0] = func_0021170C(*(f32 *) &ai->m_VarBuffer[0], temp_f20);
    }
    if (ai->m_Action == 0) {
        temp_f0 = ai->m_ActionTime - D_800B6D28;
        ai->m_ActionTime = temp_f0;
        if (temp_f0 < 0.0f) {
            ai->m_Action = 1;
        }
    }
}
