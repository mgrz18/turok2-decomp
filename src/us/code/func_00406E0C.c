#include "common.h"

extern f32 func_002119FC(f32 lo, f32 hi);

/* One step of a boss attack script (0x14 bytes). */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x04 */ s32 arg;
    /* 0x08 */ f32 animSpeed;
    /* 0x0C */ s32 chance;
    /* 0x10 */ f32 delay;
} BossStep;

/* Boss attack script cursor at 0x8012F9F8. */
typedef struct {
    /* 0x0 */ f32 cooldown;
    /* 0x4 */ BossStep *script;
    /* 0x8 */ s32 cursor;
    /* 0xC */ s32 index;
} BossScript;

extern BossScript D_8012F9F8;

/* Pick the next attack of the script: steps are skipped by chance, type 5 wraps around. */
void func_00406E0C(void) {
    if (D_8012F9F8.cursor != -1) {
        D_8012F9F8.cooldown = D_8012F9F8.script[D_8012F9F8.cursor].delay;
    } else {
        D_8012F9F8.cooldown = 0.0f;
    }
    do {
        D_8012F9F8.cursor++;
        if (D_8012F9F8.script[D_8012F9F8.cursor].type == 5) {
            D_8012F9F8.cursor = 0;
        }
        D_8012F9F8.index = D_8012F9F8.cursor;
    } while ((f32) D_8012F9F8.script[D_8012F9F8.cursor].chance <= func_002119FC(0.0f, 100.0f));
}
