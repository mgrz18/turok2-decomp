/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AUDIO/8025A3EC_4.CPP, func_8025BF08_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/sound.h"

 
void alSndpSetSound(void *sound, s16 sample);
s32 alSndpGetState(void *sound);
void alSndpStop(void *sound);

/* Stop every voice whose second key matches. */
void SndVoicePool_StopByOwner(SndVoicePool *set, s32 key) {
    s32 i;
    SndVoice *voice = set->voices;

    for (i = 0; i < 17; voice++, i++) {
        if (voice->unkB4 == key) {
            SndMgr *owner = voice->mgr;
            void *sound;

            voice->stopping = 1;
            voice->vp.follow = 0;
            if (voice->vp.frame != owner->frame) {
                sound = &owner->player;
                alSndpSetSound(sound, owner->handles[voice->index]);
                if (alSndpGetState(sound)) {
                    alSndpStop(sound);
                }
                voice->sound = -1;
            }
        }
    }
}
