#include "common.h"

/* The sound manager (see func_00270240). */
typedef struct {
    /* 0x0000 */ u8 pad0[0x2BB8];
    /* 0x2BB8 */ s32 unk2BB8;
} SndMgr;

void func_002718FC(SndMgr *arg0, s32 arg1) {
    arg0->unk2BB8 = arg1;
}
