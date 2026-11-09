#include "common.h"
#include "structs/game.h"
#include "globals/engine.h"
#include "globals/frame.h"
#include "protos/ultra.h"

extern void *memset(void *dst, s32 value, uptr size);
extern void mainproc(void *arg);

void CEngineApp__Idle(CEngineApp *arg0, void *arg1) {
    memset(gThreadStackMain, 1, sizeof(gThreadStackMain));
    /* the stack top given is gSchedulerStack, which follows gThreadStackMain */
    osCreateThread(&gThreadMain, 1, mainproc, arg1, gSchedulerStack, gThreadPriorityMain);
    osStartThread(&gThreadMain);
    {
        s32 v = gThreadPriorityIdle;

        arg0->m_Settings.ready = 0;
        osSetThreadPri(0, v);
    }
loop_1:
    goto loop_1;
}
