#include "common.h"

/* What this initialises. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s32 unk10;
} Request;

void alFilterNew(Request *arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->unk0 = 0;
    arg0->unk4 = arg1;
    arg0->unk8 = arg2;
    arg0->unkC = 0;
    arg0->unkE = 0;
    arg0->unk10 = arg3;
}
