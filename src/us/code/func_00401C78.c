#include "common.h"

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s32 state;
} Unk00401C78;

s32 func_00401C78(Unk00401C78 *arg0) {
    return (u32) (arg0->state - 1) < 2U;
}
