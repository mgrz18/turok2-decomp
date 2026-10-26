/* NEAR MATCH: only differs by the FP-compare hazard nop that the ROM has at the merged
 * "bc1fl" tail (label .L002750E0); cc1 here emits no nop there. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 handle;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ u32 flags;
    /* 0x20 */ union {
        s32 w;
        struct {
            s16 hi;
            s16 lo;
        } h;
    } volume;
    /* 0x24 */ u8 pad24[0x10];
    /* 0x34 */ f32 fadeStep;
    /* 0x38 */ s32 sliding;
    /* 0x3C */ f32 current;
    /* 0x40 */ f32 target;
} Channel;

void func_00290EC0(s32 handle, s16 volume);
void func_00290EF0(s32 handle);

void func_00275018(Channel *ch) {
    f32 current;
    f32 target;
    f32 next;

    if (ch->flags & 2) {
        func_00290EC0(ch->handle, ch->volume.h.lo);
        ch->volume.w = (f32) ch->volume.w - ch->fadeStep;
        if (ch->volume.w <= 0) {
            ch->volume.w = 0;
            func_00290EF0(ch->handle);
            ch->flags |= 4;
        }
    } else if (ch->sliding) {
        current = ch->current;
        target = ch->target;
        if (target < current) {
            next = current - 1.0f / 60;
            if (!(target <= next)) {
                next = target;
            }
        } else if (current < target) {
            next = current + 1.0f / 60;
            if (!(next <= target)) {
                next = target;
            }
        } else {
            return;
        }
        ch->current = next;
    }
}
