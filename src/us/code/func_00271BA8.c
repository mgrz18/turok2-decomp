/* Cut fixed in versions/cuts.us.txt: CAudioWorld__FindVoice really is 0x271BA8-0x271BEC (0x44 bytes): splat split it into CAudioWorld__FindVoice, func_00271BBC (its found-return) and func_00271BC4 (its loop). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AUDIO_2/802591C0.CPP, func_80258FF4_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/sound.h"

 
/* Index of the first of the seventeen voices playing id, -1 if none (or id is -1). */
s32 CAudioWorld__FindVoice(SndMgr *ctx, s32 id) {
    SndVoice *slot;
    s32 i;

    slot = ctx->pool.voices;
    if (id == -1) {
        return -1;
    }
    for (i = 0; i < 17; i++, slot++) {
        if (slot->id == id) {
            return i;
        }
    }
    return -1;
}
