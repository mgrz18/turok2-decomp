#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 state;
} Phase;

void func_00401CA8(Phase *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->state;
    if ((temp_v0 != 0) && (temp_v0 != 3)) {
        arg0->state = 3;
    }
}
