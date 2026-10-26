/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AUDIO/8025A3EC_4.CPP, func_8025BE08_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x84];
    /* 0x084 */ u8 sound[0xDC - 0x84];
    /* 0x0DC */ s16 samples[(0x104 - 0xDC) / 2];
    /* 0x104 */ s32 currentId;
} SoundOwner;

typedef struct {
    /* 0x00 */ s32 sampleIndex;
    /* 0x04 */ s32 handle;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ s32 id;
    /* 0x14 */ u8 pad14[0x3C];
    /* 0x50 */ s32 timer;
    /* 0x54 */ u8 pad54[0x50];
    /* 0xA4 */ u32 groupMask;
    /* 0xA8 */ s32 key;
    /* 0xAC */ s32 stopping;
    /* 0xB0 */ SoundOwner *owner;
    /* 0xB4 */ s32 key2;
    /* 0xB8 */ u8 padB8[0xCC - 0xB8];
} Voice; /* size 0xCC */

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Voice voices[17];
} VoiceSet;

void func_00293E60(void *sound, s16 sample);
s32 func_00293580(void *sound);
void func_00293EC0(void *sound);

/* Stop every voice whose key matches. */
void func_00274144(VoiceSet *set, s32 key) {
    s32 i;
    Voice *voice = set->voices;

    for (i = 0; i < 17; voice++, i++) {
        if (voice->key == key) {
            SoundOwner *owner = voice->owner;
            void *sound;

            voice->stopping = 1;
            voice->timer = 0;
            if (voice->id != owner->currentId) {
                sound = owner->sound;
                func_00293E60(sound, owner->samples[voice->sampleIndex]);
                if (func_00293580(sound)) {
                    func_00293EC0(sound);
                }
                voice->handle = -1;
            }
        }
    }
}
