#include "common.h"

/* A MIDI channel's state (0x10 bytes). */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s8 unk6;
    /* 0x7 */ s8 pan;
    /* 0x8 */ s8 unk8;
    /* 0x9 */ s8 volume;
    /* 0xA */ s8 unkA;
    /* 0xB */ s8 unkB;
    /* 0xC */ f32 pitchScale;
} MidiChannel;

typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ u8 numChannels;
    /* 0x35 */ u8 pad35[0x60 - 0x35];
    /* 0x60 */ s32 channels;   /* MidiChannel[numChannels] */
} MidiPlayer;

/* Resets every channel to its defaults. */
void __initChanState(MidiPlayer *arg0) {
    sptr temp_v1;
    s32 var_a1;

    var_a1 = 0;
    if (arg0->numChannels != 0) {
        do {
            temp_v1 = var_a1 * 0x10;
            ((MidiChannel *)(temp_v1 + arg0->channels))->unk0 = 0;
            ((MidiChannel *)(temp_v1 + arg0->channels))->unk6 = 0;
            ((MidiChannel *)(temp_v1 + arg0->channels))->unkA = 0;
            ((MidiChannel *)(temp_v1 + arg0->channels))->pan = 0x40;
            ((MidiChannel *)(temp_v1 + arg0->channels))->volume = 0x7F;
            ((MidiChannel *)(temp_v1 + arg0->channels))->unk8 = 5;
            ((MidiChannel *)(temp_v1 + arg0->channels))->unkB = 0;
            ((MidiChannel *)(temp_v1 + arg0->channels))->unk4 = 0xC8;
            ((MidiChannel *)(temp_v1 + arg0->channels))->pitchScale = 1.0f;
            var_a1 += 1;
        } while (var_a1 < (s32) arg0->numChannels);
    }
}
