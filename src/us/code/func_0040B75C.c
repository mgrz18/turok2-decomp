#include "common.h"

/* AI state: once the anim is done, reset every active slot of the module state, then go to anim state 0x16. */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 data[0x68 - 0x4];
} Slot;

typedef struct {
    /* 0x000 */ void *obj;
    /* 0x004 */ s32 count;
    /* 0x008 */ void *objs[4];
    /* 0x018 */ Slot slots[15];
} State;

typedef struct {
    /* 0x00 */ u8 pad0[0xC7];
    /* 0xC7 */ s8 animDone;
} CAI;

extern State D_8012FB70;
extern s32 D_801301D0; /* active slot mask */
extern u8 D_00431EE4[];
extern void func_00236314(void *dst, void *src);
extern void func_00243414(void *obj, CAI *ai, s32 state);

void func_0040B75C(void *obj, CAI *ai) {
    State *state;
    s32 mask;
    s32 i;

    if (ai->animDone != 0) {
        mask = D_801301D0;
        state = &D_8012FB70;
        for (i = 0; i < 15; i++) {
            if ((mask >> i) & 1) {
                func_00236314(state->slots[i].data, D_00431EE4);
            }
        }
        func_00243414(obj, ai, 0x16);
    }
}
