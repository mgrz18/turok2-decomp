#include "common.h"

/* The sound manager (SndCtx in func_002726DC): only the sound data tree is used here. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x2B4C];
    /* 0x2B4C */ s32 tree;
} SndCtx;

s32 func_002017D4(s32, s16);                        /* extern */

void func_00271D54(SndCtx *arg0, s16 arg1) {
    func_002017D4(func_002017D4(arg0->tree, arg1), 0);
}
