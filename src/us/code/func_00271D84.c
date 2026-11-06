#include "common.h"

s32 CISet__GetBlock(s32, s16);                        /* extern */

typedef struct {
    /* 0x0000 */ u8 pad0[0x2B4C];
    /* 0x2B4C */ s32 unk2B4C;
} Unk00271D84;

void CAudioWorld__GetVariantLayers(Unk00271D84 *arg0, s16 arg1) {
    CISet__GetBlock(CISet__GetBlock(arg0->unk2B4C, arg1), 1);
}
