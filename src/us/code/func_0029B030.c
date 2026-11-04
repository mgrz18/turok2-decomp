#include "common.h"

extern s32 D_800B8910;

typedef struct {
    /* 0x00 */ s32 *head;
    /* 0x04 */ s32 *tail;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 count;
    /* 0x14 */ s32 base;
} Unk0029B030;

void func_0029B030(Unk0029B030 *arg0, s32 arg1, s32 arg2) {
    arg0->head = &D_800B8910;
    arg0->tail = &D_800B8910;
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->count = arg2;
    arg0->base = arg1;
}
