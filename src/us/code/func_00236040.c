#include "common.h"

/* Only what this reads and writes of its two arguments. */
typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ u8 list[0xC];        /* handed to func_0026D2D0 */
    /* 0x4C */ f32 total;
} Group;

typedef struct {
    /* 0x00 */ u8 pad0[0xAC];
    /* 0xAC */ f32 amount;
} Member;

void func_0026D288(s32, Member *);                  /* extern */
void func_0026D2D0(void *, Member *);               /* extern */

/* Takes arg2 out of group arg1 (and its amount out of the group's total). */
void func_00236040(s32 arg0, Group *arg1, Member *arg2) {
    arg1->total = (f32) (arg1->total - arg2->amount);
    func_0026D2D0(arg1->list, arg2);
    func_0026D288(arg0 + 0x6A88, arg2);
}
