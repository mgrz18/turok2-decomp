#include "common.h"

s32 func_0042EF2C(s32);

extern s32 D_004384E8;
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

void func_0042F590(void) {
    Unk800F56B0 *state = &D_800F56B0;
    s32 temp_v0;

    if (state->active != 0) {
        func_0042EF2C(*(((__typeof__(&D_004384E8))((s8 *)&D_004384E8 + (state->index * 4)))));
        temp_v0 = state->index + 1;
        state->index = temp_v0;
        if (*(((__typeof__(&D_004384E8))((s8 *)&D_004384E8 + (temp_v0 * 4)))) == 0) {
            state->index = 0;
        }
    }
}
