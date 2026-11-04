#include "common.h"

/* The sound manager (see func_00270240). */
typedef struct {
    /* 0x0000 */ u8 pad0[0x2BA8];
    /* 0x2BA8 */ s32 unk2BA8;
} SndMgr;

void func_00271770(SndMgr *arg0, s32 arg1) {
    arg0->unk2BA8 = arg1;
}
