/* WRONG CUT: REAL func_0042D120 size 0x98 (splat cut it into func_0042D120 + func_0042D140). */
#include "common.h"

typedef struct IntrusiveList {
    /* 0x00 */ u8 pad0[0x14];
} IntrusiveList;

typedef struct Slot {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ u8 pad10[6];
    /* 0x16 */ s16 unk16;
} Slot; /* 0x18 */

typedef struct SlotLists {
    /* 0x00 */ IntrusiveList used;
    /* 0x14 */ IntrusiveList free;
    /* 0x28 */ Slot slots[8];
} SlotLists;

extern void CList__Construct(IntrusiveList *list, s32 prevOffset, s32 nextOffset);
extern s32 CList__AddTail(IntrusiveList *list, void *node);

void func_0042D120(SlotLists *lists) {
    s32 i;
    Slot *slot;

    CList__Construct(&lists->used, 0, 4);
    CList__Construct(&lists->free, 0, 4);
    for (i = 0; i < 8; i++) {
        slot = &lists->slots[i];
        CList__AddTail(&lists->free, slot);
        slot->unk8 = 0;
        slot->unkC = 1.0f;
        slot->unk16 = 0;
    }
}
