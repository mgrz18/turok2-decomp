#include "common.h"
#include "structs/game_object.h"
#include "rw/abu_protos.h"

f32 func_002119FC(f32 arg0, f32 arg1);

extern f32 D_800C0900;
extern f32 D_800C0904;

void func_00446C00(s32 arg0, CAI *arg1) {
    arg1->m_ConditionTimer = func_002119FC(D_800C0900, D_800C0904);
}
