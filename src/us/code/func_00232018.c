#include "common.h"

/* A record with a type at 2 (0x56 here) and flags at 0x40. */
typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u16 type;
    /* 0x04 */ u8 pad4[0x40 - 4];
    /* 0x40 */ s32 flags;
} TypedRecord;

void func_00232018(TypedRecord *arg0, s32 arg1) {
    if ((arg0->type == 0x56) && (arg1 == 2)) {
        arg0->flags |= 0x04000000;
    }
}
