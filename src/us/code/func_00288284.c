#include "common.h"
#include "structs/game.h"

s32 func_0029B060(s32, s32, s32, s32, s32, s32);
s32 func_0029B9B0(s32, s32);
s32 func_0029BB10(s32);

s32 func_00266C5C(s32 *, s32, s32); /* extern */
extern s32 D_800B6E70;
extern s32 D_800B6E84;
extern s32 D_800C7C80;
extern s32 D_800CFC80;
extern s32 D_800F6A70;
extern s32 mainproc;

void func_00288284(CEngineApp *arg0, s32 arg1) {
    func_00266C5C(&D_800C7C80, 1, 0x8000);
    func_0029B060((s32) &D_800F6A70, 1, (s32) &mainproc, arg1, (s32) &D_800CFC80, D_800B6E84);
    func_0029BB10((s32) &D_800F6A70);
    {
        s32 v = D_800B6E70;

        arg0->m_Settings.ready = 0;
        func_0029B9B0(0, v);
    }
loop_1:
    goto loop_1;
}
