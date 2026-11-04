#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x238];
    /* 0x238 */ s32 pending;
} Task;

s32 func_0029B9B0(void *, s32);                 /* extern */

extern s32 D_800B6E7C;
extern s32 D_800B6E88;

void func_00201C08(Task *arg0) {
    if (arg0->pending != 0) {
        do {
            func_0029B9B0(arg0, D_800B6E88);
        } while (arg0->pending != 0);
    }
    func_0029B9B0(arg0, D_800B6E7C);
}
