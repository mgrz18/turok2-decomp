#include "common.h"

typedef struct {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ s32 unkC;
} Unk0028F710;

s32 alCSeqGetTicks(Unk0028F710 *arg0) {
    return arg0->unkC;
}
