#include "common.h"

typedef struct {
    /* 0x0000 */ u8 pad0[0x2B74];
    /* 0x2B74 */ s32 tab74;
} SoundBank;

s32 func_00271790(SoundBank *arg0, s32 arg1) {
    return arg0->tab74 + (arg1 * 0xC);
}
