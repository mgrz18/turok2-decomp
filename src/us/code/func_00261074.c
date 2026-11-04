#include "common.h"
#include "structs/game_object.h"

/* How this actor uses CAI's m_VarBuffer: two angles turned toward 0. */
typedef struct {
    /* 0x00 */ f32 angle0;
    /* 0x04 */ f32 angle1;
} AITurnVars;

f32 func_0021170C(f32, f32);
s32 func_0025E428(s32, s32);
extern f32 D_800A7A38[];
extern f32 D_800B6D28;                              /* frame delta */

void func_00261074(s32 arg0, CAI *arg1) {
    f32 temp_f0;
    f32 temp_f20;
    s8 temp_v1;

    arg1->m_vLeashPos.y = (f32) arg1->m_vInitialPos.y;
    if ((func_0025E428(arg0, (s32) arg1) != 1) || ((temp_v1 = arg1->m_Mode, (temp_v1 != 4)) && (temp_v1 != 0xE))) {
        temp_f20 = D_800A7A38[1];
        ((AITurnVars *) arg1->m_VarBuffer)->angle1 = func_0021170C(((AITurnVars *) arg1->m_VarBuffer)->angle1, temp_f20);
        ((AITurnVars *) arg1->m_VarBuffer)->angle0 = func_0021170C(((AITurnVars *) arg1->m_VarBuffer)->angle0, temp_f20);
    }
    if (arg1->m_Action == 0) {
        temp_f0 = arg1->m_ActionTime - D_800B6D28;
        arg1->m_ActionTime = temp_f0;
        if (temp_f0 < 0.0f) {
            arg1->m_Action = 1;
        }
    }
}
