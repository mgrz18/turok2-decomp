#include "common.h"

extern void *func_00296360(void *, s16 *, s32, s32, void *); /* alMainBusPull */
extern s32 func_002964A0(s32, s32, s32);
extern void func_00296340(void *, void *, void *, s32);

/* libultra ALMainBus: an ALFilter (0x14 bytes) plus its source list. */
typedef struct {
    /* 0x00 */ u8 filter[0x14];
    /* 0x14 */ s32 sourceCount;
    /* 0x18 */ s32 maxSources;
    /* 0x1C */ s32 sources;
} ALMainBus;

/* alMainBusNew */
void func_002953B4(ALMainBus *arg0, s32 arg1, s32 arg2) {
    func_00296340(arg0, func_00296360, func_002964A0, 7);
    arg0->sourceCount = 0;
    arg0->maxSources = arg2;
    arg0->sources = arg1;
}
