/* WRONG CUT: REAL func_0042EBBC size 0x70 (splat's 0x74 includes the padding nop at 0x42EC2C before the 16-aligned func_0042EC30). */
#include "common.h"

typedef struct Lists {
    /* 0x00 */ u8 list0[0x40];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 pad44[4];
    /* 0x48 */ u8 list1[0x40];
    /* 0x88 */ s32 unk88;
} Lists;

extern u8 D_800B2C30[];
extern u8 D_800B2C40[];

extern void func_0027ACC4(void *, s32, s32);
extern void func_0027AD00(void *, void *);

void func_0042EBBC(Lists *lists) {
    lists->unk40 = 0;
    func_0027ACC4(lists->list0, 0, -40);
    func_0027AD00(lists->list0, D_800B2C30);
    lists->unk88 = 0;
    func_0027ACC4(lists->list1, 0, -20);
    func_0027AD00(lists->list1, D_800B2C40);
}
