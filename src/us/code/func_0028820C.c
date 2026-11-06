#include "common.h"
#include "globals/engine.h"
#include "globals/frame.h"

extern void osCreateThread(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri);
extern void osStartThread(OSThread *thread);
extern void *memset(void *dst, s32 value, u32 size);
extern void Engine_Idle(void *arg);

void CEngineApp__Boot(void) {
    memset(gThreadStackIdle, 0, sizeof(gThreadStackIdle));
    osCreateThread(&gThreadIdle, 0, Engine_Idle, NULL, gThreadStackIdle + sizeof(gThreadStackIdle), gThreadPriorityIdleInit);
    osStartThread(&gThreadIdle);
}
