#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ s32 unk3C;
} Arg0;

void alSndpSetSound(Arg0 *arg0, s16 arg1) {
    arg0->unk3C = (s32) arg1;
}
