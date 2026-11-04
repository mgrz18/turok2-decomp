#include "common.h"

/* The sound manager (SndCtx in func_002726DC): only the listener is used here. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x2B98];
    /* 0x2B98 */ s32 listener;
} SndCtx;

void func_002718D8(SndCtx *arg0, s32 arg1) {
    arg0->listener = arg1;
}
