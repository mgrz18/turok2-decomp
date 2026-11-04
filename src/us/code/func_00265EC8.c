#include "common.h"

/* A hit, as func_00245828 fills it in (source, damage, radius, flags, ...). */
typedef struct {
    /* 0x00 */ s32 source;
    /* 0x04 */ s32 damage;
    /* 0x08 */ s32 radius;
    /* 0x0C */ s32 flags;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[4];
} DamageEvent; /* size 0x18 */

s32 func_00265EC8(DamageEvent *arg0) {
    return arg0->flags & 0x400000;
}
