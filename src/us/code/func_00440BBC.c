#include "common.h"

/* LevelScriptState (structs/boss.h) from scriptCOn on, D_8012F9D0 (+0x3C8). */
typedef struct {
    /* 0x00 */ s8 active;
    /* 0x01 */ u8 pad1[0x50 - 0x1];
    /* 0x50 */ u8 *data;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
} LevelScriptF9D0;

extern u8 D_00431620[];
extern LevelScriptF9D0 D_8012F9D0;

void Primagen_EnterMode16_2(void) {
    D_8012F9D0.active = 1;
    D_8012F9D0.unk54 = -1;
    D_8012F9D0.unk58 = -1;
    D_8012F9D0.data = D_00431620;
}
