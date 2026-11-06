#include "common.h"

s32 osCreateThread(s32, s32, s32, s32, s32, s32);
s32 osStartThread(s32);

s32 memset(s32 *, s32, s32); /* extern */
extern s32 D_800B6E74;
extern s32 D_800C7C00;
extern s32 D_800F6840;
extern s32 Engine_Idle;

void CEngineApp__Boot(void) {
    memset(&D_800C7C00, 0, 0x80);
    osCreateThread((s32) &D_800F6840, 0, (s32) &Engine_Idle, 0, (s32) ((void *)((s8 *)&D_800C7C00 + 0x80)), D_800B6E74);
    osStartThread((s32) &D_800F6840);
}
