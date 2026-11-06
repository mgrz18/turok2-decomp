#include "common.h"
#include "structs/game.h"

s32 osCreateThread(s32, s32, s32, s32, s32, s32);
s32 osSetThreadPri(s32, s32);
s32 osStartThread(s32);

s32 memset(s32 *, s32, s32); /* extern */
extern s32 D_800B6E70;
extern s32 D_800B6E84;
extern s32 D_800C7C80;
extern s32 D_800CFC80;
extern s32 D_800F6A70;
extern s32 mainproc;

void CEngineApp__Idle(CEngineApp *arg0, s32 arg1) {
    memset(&D_800C7C80, 1, 0x8000);
    osCreateThread((s32) &D_800F6A70, 1, (s32) &mainproc, arg1, (s32) &D_800CFC80, D_800B6E84);
    osStartThread((s32) &D_800F6A70);
    {
        s32 v = D_800B6E70;

        arg0->m_Settings.ready = 0;
        osSetThreadPri(0, v);
    }
loop_1:
    goto loop_1;
}
