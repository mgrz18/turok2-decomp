#include "common.h"

/* What this reads and writes. */
typedef struct {
    /* 0x00 */ s32 handle;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 state;
    /* 0x0C */ u8 padC[0x1E - 0xC];
    /* 0x1E */ s16 sound;
} SoundSlot;

void CAudioWorld__Lock(void *aw);
void CAudioWorld__Unlock(void *aw);
void *CAudioWorld__GetSndPlayer(void *aw);
void alSndpSetSound(void *sndp, s16 id);
void alSndpStop(void *sndp);

void SpeechPlayer_Stop(SoundSlot *arg0) {
    s32 temp_v0;
    s32 temp_v0_2;

    CAudioWorld__Lock(arg0->handle);
    temp_v0 = arg0->state;
    if ((temp_v0 != 2) && (temp_v0 != 0)) {
        arg0->state = 2;
        temp_v0_2 = CAudioWorld__GetSndPlayer(arg0->handle);
        alSndpSetSound(temp_v0_2, arg0->sound);
        alSndpStop(temp_v0_2);
    }
    CAudioWorld__Unlock(arg0->handle);
}
