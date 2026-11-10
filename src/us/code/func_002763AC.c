#include "common.h"

typedef struct ListHead {
    /* 0x00 */ struct ListHead *next;
    /* 0x04 */ struct ListHead *prev;
    /* 0x08 */ u8 data[1];
} ListHead;

s32 SndVoice_Construct(sptr, sptr, s32);

void SndQueue_InitNode(ListHead *arg0, s32 arg1) {
    arg0->prev = arg0;
    arg0->next = arg0;
    SndVoice_Construct((sptr) arg0->data, arg1, -1);
}
