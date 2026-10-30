#include "common.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

extern f32 func_002119FC(f32 lo, f32 hi);

/* One step of the boss's second script (0x1C bytes). */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x04 */ s32 arg;
    /* 0x08 */ u8 pad08[0x14];
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

s32 func_004050E0(void) {
    if (D_8012F950.timer > 0.0f) {
        return 0;
    }
    if (!D_8012F950.enabled) {
        return 0;
    }
    if (D_8012F950.script == NULL) {
        return 0;
    }
    switch (D_8012F950.script[D_8012F950.current].type) {
        case 1:
            if (D_8012F950.used < 9) {
                D_8012F950.count = MIN(D_8012F950.script[D_8012F950.current].arg, 8 - D_8012F950.used);
            }
            D_8012F950.enabled = 0;
            return 1;
        case 2:
            D_8012F950.target = D_8012F950.script[D_8012F950.current].arg;
            D_8012F950.enabled = 0;
            return 1;
        case 5:
            D_8012F950.current = -1;
            return 1;
        case 3:
            if (func_002119FC(0.0f, 100.0f) < 50.0f) {
                if (D_8012F950.used < 9) {
                    D_8012F950.count = MIN(D_8012F950.script[D_8012F950.current].arg, 8 - D_8012F950.used);
                }
            } else {
                D_8012F950.target = D_8012F950.script[D_8012F950.current].arg;
            }
            D_8012F950.enabled = 0;
            return 1;
        case 4:
            break;
    }
    return 0;
}
