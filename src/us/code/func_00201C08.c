#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x238];
    /* 0x238 */ s32 pending;
} Task;

s32 osSetThreadPri(void *, s32);                 /* extern */

extern s32 D_800B6E7C;
extern s32 D_800B6E88;

void CDecompressor__Flush(Task *arg0) {
    if (arg0->pending != 0) {
        do {
            osSetThreadPri(arg0, D_800B6E88);
        } while (arg0->pending != 0);
    }
    osSetThreadPri(arg0, D_800B6E7C);
}
