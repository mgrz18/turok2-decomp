#include "common.h"

s32 CList__AddHead(void *, void *);              /* extern */
s32 CList__Remove(void *, void *);              /* extern */

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 owner;
    /* 0x0C */ u8 padC[0x16 - 0xC];
    /* 0x16 */ s16 unk16;
} PoolNode00228AEC;

/* Two CLists: the used list at 0x0 and the free list at 0x14. */
typedef struct {
    /* 0x00 */ PoolNode00228AEC *usedHead;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ PoolNode00228AEC *freeHead;
} Pool00228AEC;

void *func_00228AEC(Pool00228AEC *arg0, s32 arg1) {
    PoolNode00228AEC *temp_s0;

    temp_s0 = arg0->freeHead;
    if (temp_s0 != NULL) {
        CList__Remove(&arg0->freeHead, temp_s0);
        CList__AddHead(arg0, temp_s0);
        temp_s0->owner = arg1;
        temp_s0->unk16 = 0;
    }
    return temp_s0;
}
