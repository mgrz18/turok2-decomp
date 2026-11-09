#include "common.h"
#include "structs/sound.h"

s32 alSndpSetVol(s32, s32);
s32 alSndpSetSound(s32, s16);                    /* extern */

void SndVoice_SetVolume(SndVoice *arg0, s32 arg1) {
    CAudioWorld *temp_s0;
    sptr temp_s0_2;

    temp_s0 = arg0->mgr;
    temp_s0_2 = (sptr) &temp_s0->player;
    alSndpSetSound(temp_s0_2, temp_s0->handles[arg0->index]);
    alSndpSetVol(temp_s0_2, (s32) (s16) (s32) ((f32) arg1 * arg0->volumeScale));
}
