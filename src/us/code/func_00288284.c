#include "common.h"
#include "structs/game.h"
#include "globals/engine.h"

s32 osCreateThread(s32, s32, s32, s32, s32, s32);
s32 osSetThreadPri(s32, s32);
s32 osStartThread(s32);

s32 memset(s32 *, s32, s32); /* extern */
extern s32 gThreadStackMain;
extern s32 gSchedulerStack;
extern s32 gThreadMain;
extern s32 mainproc;

void CEngineApp__Idle(CEngineApp *arg0, s32 arg1) {
    memset(&gThreadStackMain, 1, 0x8000);
    osCreateThread((s32) &gThreadMain, 1, (s32) &mainproc, arg1, (s32) &gSchedulerStack, gThreadPriorityMain);
    osStartThread((s32) &gThreadMain);
    {
        s32 v = gThreadPriorityIdle;

        arg0->m_Settings.ready = 0;
        osSetThreadPri(0, v);
    }
loop_1:
    goto loop_1;
}
