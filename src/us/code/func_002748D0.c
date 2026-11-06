#include "common.h"
#include "structs/sound.h"

s32 alSndpDeallocate(void *, s32);

/* Releases the voice slot and resets the voice. */
void SndVoice_Release(SndVoice *arg0) {
    s32 temp_a0;
    u8 *var_s1;

    temp_a0 = (s32) arg0->mgr;
    var_s1 = (u8 *) (temp_a0 + 0x7C);       /* slots - 0x60 */
    alSndpDeallocate((void *) (temp_a0 + 0x84), *(s16 *) (var_s1 + (arg0->index << 1) + 0x60));
    var_s1 += arg0->index * 2;
    *(s16 *) (var_s1 + 0x60) = -1;          /* owner->slots[index] */
    arg0->vp.priority = 0;
    arg0->id = -1;
    arg0->unk8 = -1;
    arg0->soundId = -1;
    arg0->vp.channel = -1;
    arg0->unkB4 = -1;
}
