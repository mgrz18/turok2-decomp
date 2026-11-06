#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ s32 table;
} Arg0;

s32 CISet__GetBlock(s32, s32);

/* Entry arg1 (0x20 bytes each) of section 2 of the table at 0x58. */
s32 CScene__GetRegion(Arg0 *arg0, s32 arg1) {
    { s32 base = CISet__GetBlock(arg0->table, 2) + 8; return base + (arg1 << 5); }
}
