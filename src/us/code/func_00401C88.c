#include "common.h"

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s32 state;
} Unk00401C78;

void func_00401C88(Unk00401C78 *arg0) {
    if ((u32) (arg0->state - 1) >= 2U) {
        arg0->state = 1;
    }
}
