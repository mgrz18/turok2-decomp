#include "common.h"

/* Only what this reads and writes: a voice and its volume. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 voice;
    /* 0x18 */ u8 pad18[0x2C - 0x18];
    /* 0x2C */ f32 volume;
} SoundChannel;

void alCSPSetVol(sptr, s16);                       /* extern */

/* Sets the volume (0..1, sent as 0..32767). */
void SndSeq_SetVolume(SoundChannel *arg0, f32 arg1) {
    alCSPSetVol(arg0->voice, (s16) (s32) (arg1 * 32767.0f));
    arg0->volume = arg1;
}
