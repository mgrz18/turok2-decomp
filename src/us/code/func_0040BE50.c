#include "common.h"

/* AI state: calls func_002535E4 on the players with 900, then goes to anim state 0x1F once the anim is done. */

typedef struct {
    /* 0x00 */ u8 pad0[0xC7];
    /* 0xC7 */ s8 animDone;
} CAI;

extern u8 D_80119870[];
extern void func_002535E4(void *players, f32 value);
extern void func_00243414(void *obj, CAI *ai, s32 state);

void func_0040BE50(void *obj, CAI *ai) {
    func_002535E4(D_80119870, 900.0f);
    if (ai->animDone > 0) {
        func_00243414(obj, ai, 0x1F);
    }
}
