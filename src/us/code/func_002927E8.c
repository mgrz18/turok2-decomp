#include "common.h"

/* Only the field read here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 unk24;
} Counter;

/* How far arg1 is below the counter, or 1000 when it is past it. */
s32 func_002927E8(Counter *arg0, s32 arg1) {
    s32 v = arg0->unk24 - arg1;
    if (v >= 0) {
        return v;
    }
    return 1000;
}
