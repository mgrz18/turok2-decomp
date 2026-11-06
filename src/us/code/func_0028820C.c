#include "common.h"
#include "globals/engine.h"

s32 osCreateThread(s32, s32, s32, s32, s32, s32);
s32 osStartThread(s32);

s32 memset(s32 *, s32, s32); /* extern */
extern s32 gThreadStackIdle;
extern s32 gThreadIdle;
extern s32 Engine_Idle;

void CEngineApp__Boot(void) {
    memset(&gThreadStackIdle, 0, 0x80);
    osCreateThread((s32) &gThreadIdle, 0, (s32) &Engine_Idle, 0, (s32) ((void *)((s8 *)&gThreadStackIdle + 0x80)), gThreadPriorityIdleInit);
    osStartThread((s32) &gThreadIdle);
}
