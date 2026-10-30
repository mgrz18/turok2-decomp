#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x234];
    /* 0x234 */ void *onEvent;
    /* 0x238 */ u8 pad1[0x248 - 0x238];
    /* 0x248 */ void *onUpdate;
} Actor00402ADC;

extern u8 D_800F7078[];
extern u8 D_8012F3A0[];
s32 func_00225F6C(void *world, s32 a1, s32 a2, s32 a3, Actor00402ADC **out, s32 max);
void func_00401E30(void *state, s32 a1);
void func_00243414(s32 a0, s32 a1, s32 a2);
s32 func_00403660(s32 a0, s32 a1, s32 a2);
void func_0025E4B8(void);

void func_00402ADC(s32 a0, s32 a1) {
    Actor00402ADC *found[2];
    Actor00402ADC *actor;
    s32 count;
    s32 i;

    count = func_00225F6C(D_800F7078, 0x451, 9, -1, found, 2);
    for (i = 0; i < count; i++) {
        actor = found[i];
        actor->onEvent = func_00403660;
        actor->onUpdate = func_0025E4B8;
    }
    func_00401E30(D_8012F3A0, a0);
    func_00243414(a0, a1, 1);
}
