#include "common.h"

extern f32 func_002119FC(f32 lo, f32 hi);

/* One step of the boss's second script (0x1C bytes). */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x04 */ s32 arg;
    /* 0x08 */ u8 pad08[0x8];
    /* 0x10 */ s32 chance;
    /* 0x14 */ s32 pad14;
    /* 0x18 */ f32 delay;
} BossStep2;

/* Boss module state from 0x8012F950. */
typedef struct {
    /* 0x00 */ s32 count;         /* 0x8012F950 */
    /* 0x04 */ s32 target;        /* 0x8012F954 */
    /* 0x08 */ s32 used;          /* 0x8012F958 */
    /* 0x0C */ u8 pad0C[0xAC];
    /* 0xB8 */ f32 timer;         /* 0x8012FA08 */
    /* 0xBC */ BossStep2 *script; /* 0x8012FA0C */
    /* 0xC0 */ s32 index;         /* 0x8012FA10 */
    /* 0xC4 */ s32 current;       /* 0x8012FA14 */
    /* 0xC8 */ u8 enabled;        /* 0x8012FA18 */
} BossScript2;

extern BossScript2 D_8012F950;

/* Advance the second script to its next step, skipping steps by chance. */
void func_00404FE0(void) {
    if (D_8012F950.script == NULL) {
        return;
    }
    if (D_8012F950.index != -1) {
        D_8012F950.timer = D_8012F950.script[D_8012F950.index].delay;
    } else {
        D_8012F950.timer = 0.0f;
    }
    do {
        D_8012F950.index++;
        if (D_8012F950.script[D_8012F950.index].type == 4) {
            D_8012F950.enabled = 0;
            D_8012F950.index = -1;
            D_8012F950.current = -1;
            D_8012F950.script = NULL;
            return;
        }
        D_8012F950.current = D_8012F950.index;
    } while ((f32) D_8012F950.script[D_8012F950.index].chance <= func_002119FC(0.0f, 100.0f));
}
