#include "common.h"

/* Only the field written here (next to the one func_002718E0 reads). */
typedef struct {
    /* 0x0000 */ u8 pad0[0x2BB0];
    /* 0x2BB0 */ s32 unk2BB0;
    /* 0x2BB4 */ s32 unk2BB4;
} BigState2BB0;

void func_002718D0(BigState2BB0 *arg0, s32 arg1) {
    arg0->unk2BB4 = arg1;
}
