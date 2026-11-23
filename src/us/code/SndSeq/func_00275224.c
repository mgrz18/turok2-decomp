#include "common.h"

/* Only what this writes. */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ s32 unk40;
} Flags38;

void SndSeq_Duck(Flags38 *arg0) {
    arg0->unk38 = 1;
    arg0->unk40 = 0;
}
