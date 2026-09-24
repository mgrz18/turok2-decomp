#include "common.h"

typedef struct {
    /* 0x00 */ char pad[0x1C];
    /* 0x1C */ s32 x1C;
    /* 0x20 */ char pad2[8];
    /* 0x28 */ s32 x28;
    /* 0x2C */ char pad3[4];
} Slot; /* size 0x30 */

typedef struct {
    /* 0x00 */ char pad[0x3C];
    /* 0x3C */ s32 cur;
    /* 0x40 */ Slot *slots;
} Pool;

void func_00293530(Pool *p, s16 i) {
    if (p->slots[i].x28 == 0) {
        p->slots[i].x1C = 0;
        if (p->cur == i) {
            p->cur = -1;
        }
    }
}
