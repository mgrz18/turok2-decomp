#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ f32 unk40;
} Arg0;

void func_00275234(Arg0 *arg0) {
    arg0->unk38 = 0;
    arg0->unk40 = 1.0f;
}
