#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 body[0x68 - 0xC];
} Slot0040328C;

typedef struct {
    /* 0x000 */ Slot0040328C slots[5];
    /* 0x208 */ u8 pad1[0x214 - 0x208];
    /* 0x214 */ s32 *info;
} State0040328C;

extern State0040328C D_8012F3A0;
extern u8 D_00430C6C[];
void func_00236314(void *slot, void *arg);

void func_0040328C(void) {
    State0040328C *s = &D_8012F3A0;
    s32 index = s->info[5];

    if (index != -1) {
        func_00236314(s->slots[index].body, D_00430C6C);
    }
}
