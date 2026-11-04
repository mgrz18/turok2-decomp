#include "common.h"

typedef struct {
    /* 0x000 */ s32 active;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ s32 unkC;
    /* 0x010 */ u8 pad10[0x1C8 - 0x10];
    /* 0x1C8 */ s32 unk1C8;
    /* 0x1CC */ u8 pad1CC[0x350 - 0x1CC];
    /* 0x350 */ s32 index;    /* into the zero-terminated list D_004384E8 */
} Unk800F56B0;
extern Unk800F56B0 D_800F56B0;

void func_0042F4F0(void) {
    D_800F56B0.active = 0;
    D_800F56B0.unk4 = 0;
    D_800F56B0.unk8 = 0;
    D_800F56B0.unkC = 0;
    D_800F56B0.index = 0;
    D_800F56B0.unk1C8 = 0;
}
