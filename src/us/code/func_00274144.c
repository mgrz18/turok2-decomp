/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AUDIO/8025A3EC_4.CPP, func_8025BE08_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/sound.h"

 
void func_00293E60(void *sound, s16 sample);
s32 func_00293580(void *sound);
void func_00293EC0(void *sound);

/* Stop every voice whose key matches. */
void func_00274144(SndVoicePool *set, s32 key) {
    s32 i;
    SndVoice *voice = set->voices;

    for (i = 0; i < 17; voice++, i++) {
        if (voice->soundId == key) {
            SndMgr *owner = voice->mgr;
            void *sound;

            voice->stopping = 1;
            voice->vp.state = 0;
            if (voice->vp.frame != owner->frame) {
                sound = owner->player;
                func_00293E60(sound, owner->handles[voice->index]);
                if (func_00293580(sound)) {
                    func_00293EC0(sound);
                }
                voice->sound = -1;
            }
        }
    }
}
