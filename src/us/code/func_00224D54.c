#include "common.h"

s32 func_0020185C(s32, s32, s32, s32);
s32 func_0020367C(s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_00266D24(s32, s32, s32);

extern s32 func_00200AE0;
extern s32 D_800A5F38;

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s32 count;
    /* 0x8 */ u8 entries[1];
} Table00224D54;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x44 - 0x1C];
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad48[0x78 - 0x48];
    /* 0x78 */ Table00224D54 *table;
} Unk00224D54;

s32 func_00224D54(Unk00224D54 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp20;
    s32 temp_v0;
    Table00224D54 *temp_a0;

    temp_a0 = arg0->table;
    temp_v0 = func_00266D24((s32) temp_a0->entries, temp_a0->count, arg1);
    if (temp_v0 != -1) {
        return func_0020367C(0, func_0020185C(arg0->unk44, arg0->unk18, temp_v0, (s32) &sp20), sp20, arg2, 0, (s32) &func_00200AE0, (s32) &D_800A5F38, arg3);
    }
    return 0;
}
