#include "common.h"

typedef struct {
    /* 0x00 */ s32 param1;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ s32 param6;
} Unk002973E0;

/* Sets parameter arg1 (1 or 6) to arg2. */
s32 func_002973E0(Unk002973E0 *arg0, s32 arg1, s32 arg2) {
    switch (arg1) {
        case 1:
            arg0->param1 = arg2;
            break;
        case 6:
            arg0->param6 = arg2;
            break;
    }
    return 0;
}
