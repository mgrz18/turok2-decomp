#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ u16 sound;
} SoundEvent;

void Music_SetTrack(s32 arg0);
void Sound__PlaySpeech(s32 arg0);

void Event_PlaySoundOrMusic(s32 arg0, s32 arg1, SoundEvent *arg2) {
    if ((u16) arg2->sound >= 0x100U) {
        Sound__PlaySpeech(arg2->sound);
        return;
    }
    Music_SetTrack((s32) arg2->sound);
}
