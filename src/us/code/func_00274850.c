#include "common.h"
#include "structs/sound.h"

s32 alSndpStop(s32);
s32 alSndpGetState(void *);                          /* extern */
void alSndpSetSound(void *, s16);                    /* extern */

void SndVoice_Stop(SndVoice *arg0) {
    CAudioWorld *temp_a0;
    void *temp_s0;

    temp_a0 = arg0->mgr;
    arg0->stopping = 1;
    arg0->vp.follow = 0;
    temp_s0 = &temp_a0->player;
    if (arg0->vp.frame != temp_a0->frame) {
        alSndpSetSound(temp_s0, temp_a0->handles[arg0->index]);
        if (alSndpGetState(temp_s0) != 0) {
            alSndpStop((s32) temp_s0);
        }
        arg0->sound = -1;
    }
}
