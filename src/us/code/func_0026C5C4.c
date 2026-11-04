#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 flags;
    /* 0x48 */ u8 pad48[0x58 - 0x48];
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[0x5B - 0x59];
    /* 0x5B */ u8 unk5B;
    /* 0x5C */ s8 unk5C;
} Unk0022425C;

/* Resource entry: a key, its flags and three child entries. */
typedef struct Entry0026C5C4 {
    /* 0x00 */ u16 key;
    /* 0x02 */ u16 flags;
    /* 0x04 */ u8 pad4[0x10 - 0x4];
    /* 0x10 */ struct Entry0026C5C4 *child0;
    /* 0x14 */ struct Entry0026C5C4 *child1;
    /* 0x18 */ struct Entry0026C5C4 *child2;
} Entry0026C5C4;

s32 func_0022425C(s32, s32);

s32 func_0026CE44(s32, s32);                    /* extern */
extern s32 D_800F7078;

void func_0026C5C4(Entry0026C5C4 *arg0) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_v0;
    s32 temp_s0;
    u16 temp_v1;

    if (arg0 != NULL) {
        temp_v1 = arg0->flags;
        if (!(temp_v1 & 1)) {
            temp_a0 = (s32) arg0->child0;
            temp_s0 = arg0->key;
            arg0->flags = (u16) (temp_v1 | 1);
            if (temp_a0 != 0) {
                func_0026CE44(temp_a0, temp_s0);
            }
            temp_a0_2 = (s32) arg0->child1;
            if (temp_a0_2 != 0) {
                func_0026CE44(temp_a0_2, temp_s0);
            }
            temp_a0_3 = (s32) arg0->child2;
            if (temp_a0_3 != 0) {
                func_0026CE44(temp_a0_3, temp_s0);
            }
        }
        temp_v0 = func_0022425C((s32) &D_800F7078, (s32) arg0);
        if (temp_v0 != 0) {
            ((Unk0022425C *)temp_v0)->unk5C = 2;
            ((Unk0022425C *)temp_v0)->flags = ((Unk0022425C *)temp_v0)->flags | 1;
            ((Unk0022425C *)temp_v0)->unk5B = (u8) (((Unk0022425C *)temp_v0)->unk5B | 2);
        }
    }
}
