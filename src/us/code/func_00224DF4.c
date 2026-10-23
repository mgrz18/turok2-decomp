#include "common.h"

typedef struct Table {
    u8 pad0[4];
    s32 count;      /* 0x4 */
    u8 entries[1];  /* 0x8 */
} Table;

typedef struct Owner {
    u8 pad0[0x1C];
    s32 unk1C;      /* 0x1C */
    u8 pad20[0x48 - 0x20];
    s32 unk48;      /* 0x48 */
    u8 pad4C[0x7C - 0x4C];
    Table *table;   /* 0x7C */
} Owner;

extern s32 func_00266D24(void *entries, s32 count, s32 id);
extern s32 func_0020185C(s32 arg0, s32 arg1, s32 index, s32 *out);
extern s32 func_0020367C(s32 arg0, s32 arg1, s32 arg2, s32 flags, s32 arg4, void *arg5, void *arg6, s32 arg7);
extern u8 D_00200AE0[];
extern u8 D_800A5F48[];

s32 func_00224DF4(Owner *owner, s32 id, s32 flags, s32 arg3) {
    s32 index;
    s32 out;

    index = func_00266D24(owner->table->entries, owner->table->count, id);
    if (index == -1) {
        return 0;
    }
    switch (id) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
        flags |= 0x18;
        break;
    default:
        flags |= 4;
        break;
    }
    return func_0020367C(0, func_0020185C(owner->unk48, owner->unk1C, index, &out), out, flags, 0,
                         D_00200AE0, D_800A5F48, arg3);
}
