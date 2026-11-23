#include "common.h"
#include "globals/engine.h"
#include "globals/frame.h"
#include "protos/ultra.h"

extern void *memset(void *dst, s32 value, uptr size);
extern void Engine_Idle(void *arg);

void CEngineApp__Boot(void) {
    memset(gThreadStackIdle, 0, sizeof(gThreadStackIdle));
    osCreateThread(&gThreadIdle, 0, Engine_Idle, NULL, gThreadStackIdle + sizeof(gThreadStackIdle), gThreadPriorityIdleInit);
    osStartThread(&gThreadIdle);
}
