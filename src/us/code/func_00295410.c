#include "common.h"

extern s32 func_00294D54(s32, s32, s32, s32, s32);
extern s32 func_00294E3C(s32, s32, s32);
extern void func_00296340(void *, void *, void *, s32);

/* libultra ALAuxBus: an ALFilter (0x14 bytes) plus its source list. */
typedef struct {
    /* 0x00 */ u8 filter[0x14];
    /* 0x14 */ s32 sourceCount;
    /* 0x18 */ s32 maxSources;
    /* 0x1C */ s32 sources;
} ALAuxBus;

/* alAuxBusNew */
void func_00295410(ALAuxBus *arg0, s32 arg1, s32 arg2) {
    func_00296340(arg0, (s8 *)func_00294D54 + 0xC, func_00294E3C, 6);
    arg0->sourceCount = 0;
    arg0->maxSources = arg2;
    arg0->sources = arg1;
}
