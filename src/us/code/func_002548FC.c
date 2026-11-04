#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xA08];
    /* 0xA08 */ s16 state;
} Player;

s32 func_002548FC(Player *arg0) {
    s16 temp_v1;

    temp_v1 = arg0->state;
    if ((temp_v1 == 0x12) || (temp_v1 == 0x10) || (temp_v1 == 0x11)) {
        return 1;
    }
    return 0;
}
