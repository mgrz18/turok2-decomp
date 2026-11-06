#include "common.h"
#include "globals/engine.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x238];
    /* 0x238 */ s32 pending;
} Task;

s32 osSetThreadPri(void *, s32);                 /* extern */


void CDecompressor__Flush(Task *arg0) {
    if (arg0->pending != 0) {
        do {
            osSetThreadPri(arg0, gThreadPriorityDecompressorHigh);
        } while (arg0->pending != 0);
    }
    osSetThreadPri(arg0, gThreadPriorityDecompressorLow);
}
