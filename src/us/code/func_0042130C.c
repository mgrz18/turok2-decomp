#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x684];
    /* 0x684 */ s32 unk684;
} Entry;

extern s32 D_800F70A8;          /* base address; arg0 is a byte offset into it */

void func_0042130C(s32 arg0, s32 arg1) {
    ((Entry *) (D_800F70A8 + arg0))->unk684 = arg1;
}
