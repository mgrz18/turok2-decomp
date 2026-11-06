#include "common.h"
#include "structs/game_object.h"
#include "structs/sound.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ SndSource *sounds;
    /* 0x18 */ u8 pad18[0x28 - 0x18];
    /* 0x28 */ s32 playing;
} SoundOwner;

s32 Sound__Play(s16, CVector3, s32, s32);    /* extern */
extern s32 sLoopSoundsLastResumed;

/* Restarts every looping sound of the owner. */
void LoopSounds_Resume(SoundOwner *arg0) {
    s32 temp_v0;
    SndSource *var_s0;

    var_s0 = arg0->sounds;
    if (var_s0 != NULL) {
        do {
            temp_v0 = Sound__Play(var_s0->sound.h.lo, var_s0->pos, var_s0->flags, -1);
            var_s0->handle = temp_v0;
            var_s0 = var_s0->next;
            sLoopSoundsLastResumed = temp_v0;
        } while (var_s0 != NULL);
    }
    arg0->playing = 0;
}
