#include "common.h"

/* The sound manager (see func_00270240): the field this sets. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x2BA4];
    /* 0x2BA4 */ f32 unk2BA4;
} SndMgr;

void func_00271748(SndMgr *arg0, u32 arg1) {
    arg0->unk2BA4 = arg1;
}
