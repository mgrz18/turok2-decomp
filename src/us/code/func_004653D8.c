#include "common.h"
#include "rw/abu_protos.h"

void func_0042C620(void *, int, int);

/* The world: the current level and the field this clears. */
typedef struct World {
    /* 0x00000 */ u8 pad0[0x18FA8];
    /* 0x18FA8 */ s32 level;
    /* 0x18FAC */ u8 pad18FAC[0x18FB8 - 0x18FAC];
    /* 0x18FB8 */ s32 unk18FB8;
} World;

void func_004653D8(World *arg0, s32 arg1) {
    if (arg1 != arg0->level) {
        arg0->unk18FB8 = 0;
        func_0042C620(arg0, ~arg1, 0);
    }
}
