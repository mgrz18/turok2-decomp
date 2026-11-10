#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x78];
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s32 unk7C;
} Target;

void AI_SetTargetOverride(sptr arg0, Target *arg1, sptr arg2, s32 arg3) {
    arg1->unk78 = arg2;
    arg1->unk7C = arg3;
}
