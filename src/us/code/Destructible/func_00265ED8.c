#include "common.h"

/* Only the word read here. */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u32 flags;
} FlagsRecord;

/* Whether flag bit 22 is clear. */
s32 Destructible_IsHitNotExplosive(FlagsRecord *arg0) {
    return (((u32) arg0->flags >> 0x16) ^ 1) & 1;
}
