#include "common.h"

/* The state at D_8012F9D0. */
typedef struct {
    /* 0x00 */ s8 active;
    /* 0x01 */ u8 pad1[0x50 - 0x1];
    /* 0x50 */ u8 *data;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
} BossState;

extern u8 D_00431620[];
extern BossState D_8012F9D0;

void func_00440BBC(void) {
    D_8012F9D0.active = 1;
    D_8012F9D0.unk54 = -1;
    D_8012F9D0.unk58 = -1;
    D_8012F9D0.data = D_00431620;
}
