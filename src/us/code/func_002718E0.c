#include "common.h"

/* Only the field read here. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x2BB0];
    /* 0x2BB0 */ s32 unk2BB0;
} BigState2BB0;

s32 func_002718E0(BigState2BB0 *arg0) {
    return arg0->unk2BB0;
}
