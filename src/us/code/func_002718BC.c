#include "common.h"

typedef struct {
    /* 0x0000 */ u8 pad0[0x2BAC];
    /* 0x2BAC */ s32 unk2BAC;
} SoundBank;

void func_002718BC(SoundBank *arg0, s32 arg1) {
    arg0->unk2BAC = arg1;
}
