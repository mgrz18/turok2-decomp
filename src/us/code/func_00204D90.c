#include "common.h"

/* A queued request on the D_800D81F4 list (protected by the D_800D8DB0
 * message queue): sorted by key, with a busy count and state flags. */
typedef struct CRequest {
    /* 0x00 */ u32 key;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 count;
    /* 0x0C */ s32 flags;
    /* 0x10 */ u8 pad10[0x24 - 0x10];
    /* 0x24 */ struct CRequest *next;
} CRequest;

s32 func_00204D90(CRequest *arg0) {
    return arg0->flags & 0x100;
}
