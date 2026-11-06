#include "common.h"

extern s32 __osThreadTail;

typedef struct {
    /* 0x00 */ s32 *head;
    /* 0x04 */ s32 *tail;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 count;
    /* 0x14 */ s32 base;
} Unk0029B030;

void osCreateMesgQueue(Unk0029B030 *arg0, s32 arg1, s32 arg2) {
    arg0->head = &__osThreadTail;
    arg0->tail = &__osThreadTail;
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->count = arg2;
    arg0->base = arg1;
}
