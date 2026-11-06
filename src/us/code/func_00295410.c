#include "common.h"
#include "structs/sound.h"

extern s32 func_00294D54(s32, s32, s32, s32, s32);
extern s32 alAuxBusParam(s32, s32, s32);
extern void alFilterNew(void *, void *, void *, s32);

/* alAuxBusNew */
void alAuxBusNew(ALAuxBus *arg0, s32 arg1, s32 arg2) {
    alFilterNew(arg0, (s8 *)func_00294D54 + 0xC, alAuxBusParam, 6);
    arg0->sourceCount = 0;
    arg0->maxSources = arg2;
    arg0->sources = arg1;
}
