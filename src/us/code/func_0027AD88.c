#include "common.h"

/* The state func_0027AD00 sets up: a value and a mode, and two more words. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 value;
    /* 0x08 */ s32 mode;
    /* 0x0C */ u8 padC[0x40 - 0xC];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
} ModeState;

void func_0027AD88(ModeState *arg0, s32 arg1) {
    arg0->unk44 = arg1;
}
