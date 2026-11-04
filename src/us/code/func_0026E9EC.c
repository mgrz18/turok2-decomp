#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s32 unk38;
} Arg0;

void func_0026E9EC(Arg0 *arg0, s32 arg1) {
    arg0->unk38 = arg1;
}
