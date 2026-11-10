#include "common.h"

/* What this reads and writes. */
typedef struct {
    /* 0x00 */ s32 handle;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 state;
    /* 0x0C */ u8 padC[0x1E - 0xC];
    /* 0x1E */ s16 sound;
} SoundSlot;

void CAudioWorld__Lock(sptr);                            /* extern */
void CAudioWorld__Unlock(sptr);                            /* extern */
s32 CAudioWorld__GetSndPlayer(s32);                             /* extern */
void alSndpSetSound(sptr, s16);                       /* extern */
void alSndpStop(sptr);                            /* extern */

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
