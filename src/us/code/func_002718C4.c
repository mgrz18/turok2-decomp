#include "common.h"

typedef struct {
    /* 0x0000 */ u8 pad0[0x2BAC];
    /* 0x2BAC */ s32 unk2BAC;
} SoundBank;

s32 func_002718C4(SoundBank *arg0) {
    return arg0->unk2BAC;
}
