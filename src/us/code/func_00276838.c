#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s8 unk4;
} Arg;

s8 func_00276838(Arg *arg) {
    return arg->unk4;
}
