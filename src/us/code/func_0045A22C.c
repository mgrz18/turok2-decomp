#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x684];
    /* 0x684 */ s32 value;
} Slot;

extern s32 D_800F70A8;

void Pak_SetBufferSize_2(s32 arg0, s32 arg1) {
    ((Slot *) (D_800F70A8 + arg0))->value = arg1;
}
