#include "common.h"

extern s32 D_00430F28;
extern s32 D_00430FB4;
extern s32 D_00431004;
extern s32 D_004310B8;
extern s32 D_00431158;
extern s32 D_00431220;
/* Boss module state; a stage change restarts the attack script for the new stage. */
typedef struct {
    /* 0x000 */ s32 stage;
    /* 0x004 */ u8 pad4[0x3F4 - 0x4];
    /* 0x3F4 */ s32 *script;
    /* 0x3F8 */ s32 scriptPrev;
    /* 0x3FC */ s32 scriptIndex;
} BossState;

extern BossState D_8012F608;

void func_0043FF10(s32 arg0) {
    if (arg0 == 2) {
        if (D_8012F608.stage == 1) {
            D_8012F608.script = &D_00430FB4;
        }
        if (D_8012F608.stage == arg0) {
            D_8012F608.script = &D_004310B8;
        }
        if (D_8012F608.stage == 3) {
            D_8012F608.script = &D_00431220;
        }
        D_8012F608.scriptPrev = -1;
        D_8012F608.scriptIndex = -1;
    }
    if (arg0 == 1) {
        if (D_8012F608.stage == arg0) {
            D_8012F608.script = &D_00430F28;
        }
        if (D_8012F608.stage == 2) {
            D_8012F608.script = &D_00431004;
        }
        if (D_8012F608.stage == 3) {
            D_8012F608.script = &D_00431158;
        }
        D_8012F608.scriptPrev = -1;
        D_8012F608.scriptIndex = -1;
    }
}
