#include "common.h"
#include "structs/sound.h"

extern void *func_00296360(void *, s16 *, s32, s32, void *); /* alMainBusPull */
extern s32 func_002964A0(s32, s32, s32);
extern void func_00296340(void *, void *, void *, s32);

/* alMainBusNew */
void func_002953B4(ALMainBus *arg0, s32 arg1, s32 arg2) {
    func_00296340(arg0, func_00296360, func_002964A0, 7);
    arg0->sourceCount = 0;
    arg0->maxSources = arg2;
    arg0->sources = arg1;
}
