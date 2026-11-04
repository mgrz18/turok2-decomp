#include "common.h"

/* The sound manager (see func_00270240): its 0xC-byte entry table. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x2B70];
    /* 0x2B70 */ s32 entries;
} SndMgr;

s32 func_00271778(SndMgr *arg0, s32 arg1) {
    return arg0->entries + (arg1 * 0xC);
}
