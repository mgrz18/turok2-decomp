#include "common.h"

/* Only the field read here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 *unk10;
} Holder;

s32 CAnimationState__GetData(Holder *arg0) {
    return *arg0->unk10;
}
