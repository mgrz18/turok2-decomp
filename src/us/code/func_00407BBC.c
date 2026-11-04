#include "common.h"

/* Boss attack script state (0x8012F9D0 on). */
typedef struct {
    /* 0x00 */ s8 restart;
    /* 0x01 */ u8 pad1[0x50 - 0x1];
    /* 0x50 */ s32 *script;
    /* 0x54 */ s32 scriptPrev;
    /* 0x58 */ s32 scriptIndex;
} BossScript;

extern s32 D_00431700;
extern BossScript D_8012F9D0;

void func_00407BBC(void) {
    D_8012F9D0.restart = 1;
    D_8012F9D0.scriptPrev = -1;
    D_8012F9D0.scriptIndex = -1;
    D_8012F9D0.script = &D_00431700;
}
