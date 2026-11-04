#include "common.h"

/* Only what this reads and writes: a voice and its volume. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 voice;
    /* 0x18 */ u8 pad18[0x2C - 0x18];
    /* 0x2C */ f32 volume;
} SoundChannel;

void func_00290EC0(s32, s16);                       /* extern */

/* Sets the volume (0..1, sent as 0..32767). */
void func_00275324(SoundChannel *arg0, f32 arg1) {
    func_00290EC0(arg0->voice, (s16) (s32) (arg1 * 32767.0f));
    arg0->volume = arg1;
}
