/* NEAR MATCH: only differs by the FP-compare hazard nop that the ROM has at the merged
 * "bc1fl" tail (label .L002750E0); cc1 here emits no nop there. */
#include "common.h"
#include "structs/sound.h"

void func_00290EC0(s32 handle, s16 volume);
void func_00290EF0(s32 handle);

void func_00275018(SndSeq *ch) {
    f32 current;
    f32 target;
    f32 next;

    if (ch->flags & 2) {
        func_00290EC0(ch->csp, ch->volume.h.lo);
        ch->volume.w = (f32) ch->volume.w - ch->fadeStep;
        if (ch->volume.w <= 0) {
            ch->volume.w = 0;
            func_00290EF0(ch->csp);
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
