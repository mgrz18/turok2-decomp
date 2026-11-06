#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ s32 unk2C;
} Arg;

s32 alCSPGetState(Arg *arg) {
    return arg->unk2C;
}
