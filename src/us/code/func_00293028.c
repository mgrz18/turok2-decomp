#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
} Arg;

s32 func_00293028(Arg *arg) {
    return arg->unkC;
}
