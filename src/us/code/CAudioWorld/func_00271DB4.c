#include "common.h"
#include "structs/sound.h"

/* Busy: the mute flag with sound 0x44C, a pending cue, or 6 voices already playing sound id. */
s32 CAudioWorld__IsSoundBusy(CAudioWorld *m, s32 id) {
    s32 count;
    s32 i;

    if ((m->muted != 0 && id == 0x44C) || m->pendingCue != -1) {
        return 1;
    }
    count = 0;
    if (id < 0x100) {
        for (i = 0; i < 17; i++) {
            if (m->pool.voices[i].soundId == id) {
                count++;
            }
        }
        if (count >= 6) {
            return 1;
        }
    }
    return 0;
}
