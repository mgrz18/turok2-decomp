#include "common.h"

extern f32 func_002119FC(f32 lo, f32 hi);

/* One step of the boss's third script (0x14 bytes). */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x04 */ s32 arg;
    /* 0x08 */ s32 chance;
    /* 0x0C */ f32 delay;
    /* 0x10 */ f32 delayRange;
} BossStep3;

/* Boss module state from 0x8012FA1C. */
typedef struct {
    /* 0x00 */ f32 timer;         /* 0x8012FA1C */
    /* 0x04 */ BossStep3 *script; /* 0x8012FA20 */
    /* 0x08 */ s32 index;         /* 0x8012FA24 */
    /* 0x0C */ s32 current;       /* 0x8012FA28 */
} BossScript3;

extern BossScript3 D_8012FA1C;

/* Advance the third script to its next step, skipping steps by chance. */
void func_00405288(void) {
    if (D_8012FA1C.script == NULL) {
        return;
    }
    if (D_8012FA1C.index != -1) {
        D_8012FA1C.timer = D_8012FA1C.script[D_8012FA1C.index].delay
                         + func_002119FC(0.0f, D_8012FA1C.script[D_8012FA1C.index].delayRange) * 15.0f;
    } else {
        D_8012FA1C.timer = 0.0f;
    }
    do {
        D_8012FA1C.index++;
        if (D_8012FA1C.script[D_8012FA1C.index].type == 4) {
            D_8012FA1C.index = -1;
            D_8012FA1C.current = -1;
            D_8012FA1C.script = NULL;
            return;
        }
        D_8012FA1C.current = D_8012FA1C.index;
    } while ((f32) D_8012FA1C.script[D_8012FA1C.index].chance <= func_002119FC(0.0f, 100.0f));
}
