#include "common.h"

/* Only what this reads and writes of its two arguments. */
typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ u8 list[0xC];        /* handed to DList_Remove */
    /* 0x4C */ f32 total;
} Group;

typedef struct {
    /* 0x00 */ u8 pad0[0xAC];
    /* 0xAC */ f32 amount;
} Member;

void DList_AddTail(s32, Member *);                  /* extern */
void DList_Remove(void *, Member *);               /* extern */

/* Takes arg2 out of group arg1 (and its amount out of the group's total). */
void SwooshGroup_Remove(s32 arg0, Group *arg1, Member *arg2) {
    arg1->total = (f32) (arg1->total - arg2->amount);
    DList_Remove(arg1->list, arg2);
    DList_AddTail(arg0 + 0x6A88, arg2);
}
