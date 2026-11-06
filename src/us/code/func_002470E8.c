#include "common.h"

/* A record of fourteen words: all cleared but the one at 0x10, set to arg1. */
typedef struct {
    /* 0x00 */ s32 unk0[4];
    /* 0x10 */ s32 value;
    /* 0x14 */ s32 unk14[9];
} Record14;

void CPlayerContState__Init(Record14 *arg0, s32 arg1) {
    arg0->unk0[0] = 0;
    arg0->unk0[1] = 0;
    arg0->unk0[2] = 0;
    arg0->unk0[3] = 0;
    arg0->value = arg1;
    arg0->unk14[0] = 0;
    arg0->unk14[1] = 0;
    arg0->unk14[2] = 0;
    arg0->unk14[3] = 0;
    arg0->unk14[4] = 0;
    arg0->unk14[5] = 0;
    arg0->unk14[6] = 0;
    arg0->unk14[7] = 0;
    arg0->unk14[8] = 0;
}
