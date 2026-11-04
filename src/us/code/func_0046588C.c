#include "common.h"

s32 CList__AddHead(s32, s32);
s32 CList__Remove(s32, s32);
s32 func_0026DFB0(s32);

typedef struct ListNode0046588C {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ struct ListNode0046588C *next;
    /* 0x8 */ s32 handle;
} ListNode0046588C;

/* Owner with an active list at 0x1180 and a free list at 0x1194. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x1180];
    /* 0x1180 */ ListNode0046588C *activeHead;  /* CList */
    /* 0x1184 */ u8 pad1184[0x1194 - 0x1184];
    /* 0x1194 */ u8 freeList[0x14];
} Unk0046588C;

void func_0046588C(Unk0046588C *arg0) {
    ListNode0046588C *temp_s0;
    ListNode0046588C *var_s1;

    var_s1 = arg0->activeHead;
    if (var_s1 != NULL) {
        do {
            temp_s0 = var_s1->next;
            func_0026DFB0(var_s1->handle);
            CList__Remove((s32) &arg0->activeHead, (s32) var_s1);
            CList__AddHead((s32) arg0->freeList, (s32) var_s1);
            var_s1 = temp_s0;
        } while (var_s1 != NULL);
    }
}
