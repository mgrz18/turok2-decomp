#include "common.h"
#include "structs/sound.h"

extern s32 func_00294D54(s32, s32, s32, s32, s32);
#ifdef TARGET_PORT
extern Acmd *alAuxBusPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
#endif
extern s32 alAuxBusParam(s32, s32, s32);
extern void alFilterNew(void *, void *, void *, s32);

/* alAuxBusNew */
void alAuxBusNew(ALAuxBus *arg0, s32 arg1, s32 arg2) {
#ifdef TARGET_PORT
    /* func_00294D54 + 0xC is alAuxBusPull (func_00294D60): the ROM points into a stray word */
    alFilterNew(arg0, alAuxBusPull, alAuxBusParam, 6);
#else
    alFilterNew(arg0, (s8 *)func_00294D54 + 0xC, alAuxBusParam, 6);
#endif
    arg0->sourceCount = 0;
    arg0->maxSources = arg2;
    arg0->sources = arg1;
}
