#include "common.h"
#include "structs/game_object.h"
#include "rw/abu_protos.h"

f32 func_002119FC(f32 arg0, f32 arg1);
s32 func_002613D8(s32, s32);

extern f32 D_800A7A34;
extern f32 D_800A7A38;

void func_00260FF8(s32 arg0, CAI *arg1) {
    f32 temp_f20;
    f32 temp_f21;

    temp_f20 = D_800A7A34;
    temp_f21 = D_800A7A38;
    arg1->m_pfCollision = (void *) func_002613D8;
    arg1->m_vTargetOffset.x = func_002119FC(temp_f20, temp_f21);
    arg1->m_vTargetOffset.y = func_002119FC(temp_f20, temp_f21);
    arg1->m_vTargetOffset.z = func_002119FC(temp_f20, temp_f21);
}
