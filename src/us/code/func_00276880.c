#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xF];
    /* 0x0F */ s8 unkF;
} Arg;

s8 func_00276880(Arg *arg) {
    return arg->unkF;
}
