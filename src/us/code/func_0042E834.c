/* Player code: func_0042E834, 0x0042E834-0x0042E894 (splat had it inside the data D_0042D50C). */
#include "common.h"

typedef struct PlayerList {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 list0[0x14];
    /* 0x20 */ u8 list1[0x14];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
} PlayerList;

extern void func_00266C5C(void *p, s32 c, s32 n); /* memset */
extern void CList__Construct(void *list, s32 prevOffset, s32 nextOffset);

void func_0042E834(PlayerList *pl) {
    func_00266C5C(pl, 0, sizeof(PlayerList));
    CList__Construct(pl->list0, 0x131C, 0x1320);
    CList__Construct(pl->list1, 0x131C, 0x1320);
    pl->unk0 = 0;
    pl->unk4 = 0;
    pl->unk8 = 0;
    pl->unk34 = 0;
    pl->unk38 = 0;
}
