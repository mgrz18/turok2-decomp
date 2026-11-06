#include "common.h"
#include "globals/pak.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 *data;
} Item;

extern s32 D_800B490C;

s32 PakMenu_UpdateCancel_2(Item *arg0) {
    if (gPakMenuAnyPak != 0) {
        arg0->data = &D_800B490C;
    }
    return 0;
}
