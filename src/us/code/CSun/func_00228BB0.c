#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} CSunFrameData;

void CSunFrameData__Construct(CSunFrameData *arg0) {
    arg0->unk14 = 0;
    arg0->unk18 = 0;
}
