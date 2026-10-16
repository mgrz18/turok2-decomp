/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/seqplayer_802B6BE4.c, func_802B1D24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct ALNoteEvent;
struct ALOscEvent;
struct ALVolumeEvent;
struct ALEndEvent;
struct ALTempoEvent;
struct ALEvent;
typedef struct ALNoteEvent ALNoteEvent;
typedef struct ALOscEvent ALOscEvent;
typedef struct ALVolumeEvent ALVolumeEvent;
typedef struct ALEndEvent ALEndEvent;
typedef struct ALTempoEvent ALTempoEvent;
typedef struct ALEvent ALEvent;
struct ListLink;
struct ListLink {
    struct ListLink *next;
    struct ListLink *prev;
};
struct ListLink;
typedef struct ListLink ListLink;
struct ALEvent;
struct ALVoice_s;
typedef struct ALVoice_s ALVoice_s;
struct ALVoice_s;
struct ALVoice_s {
    ListLink node;
    void *pvoice;
    void *table;
    void *clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
};
struct ALMIDIEvent;
typedef struct ALMIDIEvent ALMIDIEvent;
struct InventorySlot;
struct InventorySlot {
    u8 available;
    u8 slot;
};
struct InventorySlot;
typedef struct InventorySlot InventorySlot;
struct ALMIDIEvent;
struct ALMIDIEvent {
    s32 ticks;
    u8 status;
    u8 byte1;
    u8 byte2;
    u32 duration;
};
struct func_80254D70_S2;
typedef struct func_80254D70_S2 func_80254D70_S2;
struct Request;
struct Request {
    s16 type;
};
struct func_80284AF4_G2;
typedef struct func_80284AF4_G2 func_80284AF4_G2;
struct func_80254D70_S2;
struct func_80254D70_S2 {
    char pad0[0x8];
    s32 unk8;
};
struct Request;
typedef struct Request Request;
struct func_80284AF4_G2;
struct func_80284AF4_G2 {
    void * unk0;
};
struct ALEndEvent {
    s32 ticks;
    u8 status;
    u8 type;
    u8 len;
};
struct ALTempoEvent {
    s32 ticks;
    u8 status;
    u8 type;
    u8 len;
    u8 byte1;
    u8 byte2;
    u8 byte3;
};
struct ALNoteEvent {
    struct ALVoice_s *voice;
};
struct ALVolumeEvent {
    struct ALVoice_s *voice;
    s32 delta;
    u8 vol;
};
struct ALOscEvent {
    struct ALVoiceState *vs;
    void *oscState;
    u8 chan;
};
struct ALEvent {
    s16 type;
    union {
        ALMIDIEvent midi;
        ALTempoEvent tempo;
        ALEndEvent end;
        ALNoteEvent note;
        ALVolumeEvent vol;
        Request spvol;
        InventorySlot sppriority;
        func_80284AF4_G2 spseq;
        func_80284AF4_G2 spbank;
        ALOscEvent osc;
    } msg;
};
struct ALEvent;
struct ALEndEvent;
struct ALTempoEvent;
struct ALEvent;
struct ALKeyMap;
struct ALKeyMap {
    u8 velocityMin;
    u8 velocityMax;
    u8 keyMin;
    u8 keyMax;
    u8 keyBase;
    s8 detune;
};
struct ALVoiceState;
struct ALSound_s;
typedef struct ALSound_s ALSound_s;
struct ALKeyMap;
typedef struct ALKeyMap ALKeyMap;
struct ALEnvelope;
struct ALEnvelope {
    s32 attackTime;
    s32 decayTime;
    s32 releaseTime;
    u8 attackVolume;
    u8 decayVolume;
};
struct ALEnvelope;
typedef struct ALEnvelope ALEnvelope;
struct ALSound_s;
struct ALSound_s {
    ALEnvelope *envelope;
    ALKeyMap *keyMap;
    void *wavetable;
    u8 samplePan;
    u8 sampleVolume;
    u8 flags;
};
struct ObjectLinks88;
typedef struct ObjectLinks88 ObjectLinks88;
struct ALVoiceState;
typedef struct ALVoiceState ALVoiceState;
struct ALVoice_s;
struct ALVoiceState;
struct ALVoiceState {
    struct ALVoiceState *next;
    ALVoice_s voice;
    ALSound_s *sound;
    s32 envEndTime;
    f32 pitch;
    f32 vibrato;
    u8 envGain;
    u8 channel;
    u8 key;
    u8 velocity;
    u8 envPhase;
    u8 phase;
    u8 tremelo;
    u8 flags;
};
struct ALOscEvent;
struct ALVoiceState;
struct ALVoiceState;
struct ALVoiceState;
struct ALVoiceState;
struct ALVoiceState;
struct ObjectLinks88;
struct ObjectLinks88 {
    char pad0[0x18];
    void * unk_18;
    char pad18[0x24 - 0x18 - sizeof(void*)];
    s32 unk_24;
    char pad24[0x2C - 0x24 - sizeof(s32)];
    s32 unk_2C;
    char pad2C[0x48 - 0x2C - sizeof(s32)];
    char unk_48;
    char pad48[0x7C - 0x48 - sizeof(char)];
    void * unk_7C;
    char pad7C[0x80 - 0x7C - sizeof(void*)];
    char * unk_80;
    char pad80[0x84 - 0x80 - sizeof(char*)];
    s32 unk_84;
};
struct ALNoteEvent;
struct ALOscEvent;
struct ALVolumeEvent;
struct ALNoteEvent;
struct ALVoice_s;
struct ALEvent;
extern void func_00292C84(void *arg0);
extern s32 func_00291034(void *, s16 *, s32);
extern s32 func_00293254(void *arg0, s32 *arg1);
extern s32 func_00293028(void *arg0);
extern void func_0029321C(void *arg0, void *arg1);
void func_00292C84(void *arg0) {
    ALEvent sp10;
    s32 sp20;
    s32 temp_v1;
    void *temp_s0;
    temp_s0 = ((ObjectLinks88 *)(arg0))->unk_18;
    if ((((ObjectLinks88 *)(arg0))->unk_2C == 1) && (temp_s0 != 0) &&
        (func_00293254(temp_s0, &sp20) & 0xFF)) {
        if ((((ObjectLinks88 *)(arg0))->unk_84 != 0) &&
            ((func_00293028(temp_s0) + sp20) >=
             ((struct func_80254D70_S2 *) ((ObjectLinks88 *) arg0)->unk_80)->unk8)) {
            func_0029321C(temp_s0, ((ObjectLinks88 *)(arg0))->unk_7C);
            temp_v1 = ((ObjectLinks88 *)(arg0))->unk_84;
            if (temp_v1 != -1) {
                ((ObjectLinks88 *)(arg0))->unk_84 = temp_v1 - 1;
            }
        }
        sp10.type = 0;
        func_00291034(&((ObjectLinks88 *)(arg0))->unk_48, &sp10,
                      sp20 * ((ObjectLinks88 *)(arg0))->unk_24);
    }
}
