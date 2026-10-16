/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_7.c, func_802B2D80_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct ALEventQueue;
typedef struct ALEventQueue ALEventQueue;
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
struct ALEventQueue;
struct ALEventQueue {
    ListLink freeList;
    ListLink allocList;
    s32 eventCount;
};
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
struct ParamsRecAt8V1;
typedef struct ParamsRecAt8V1 ParamsRecAt8V1;
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
struct ALPlayer_s;
typedef struct ALPlayer_s ALPlayer_s;
struct ALPlayer_s;
struct ALPlayer_s {
    struct ALPlayer_s *next;
    void *clientData;
    void *handler;
    s32 callTime;
    s32 samplesLeft;
};
struct Request;
struct Request {
    s16 type;
};
struct func_80284AF4_G2;
typedef struct func_80284AF4_G2 func_80284AF4_G2;
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
struct ParamsRecAt8V1;
struct ParamsRecAt8V1 {
    s16 f0;
    s32 f4;
    s16 f8;
};
struct ALEndEvent;
struct ALTempoEvent;
struct ALEvent;
struct ALSndPlayer;
struct ALSndPlayer {
    ALPlayer_s node;
    ALEventQueue evtq;
    ALEvent nextEvent;
    void *drvr;
    s32 target;
    void *sndState;
    s32 maxSounds;
    s32 frameTime;
    s32 nextDelta;
    s32 curTime;
};
struct ALSndPlayer;
struct ALSndPlayer;
typedef struct ALSndPlayer ALSndPlayer;
struct ALSoundState;
typedef struct ALSoundState ALSoundState;
struct ALSoundState;
struct ALSoundState {
    ALVoice_s voice;
    void *sound;
    s16 priority;
    f32 pitch;
    s32 state;
    s16 vol;
    u8 pan;
    u8 fxMix;
};
extern s32 func_00291034(void *, s16 *, s32);
void func_00293CE0(ALSndPlayer *arg0) {
    s32 base = (s32)arg0->sndState;
    if (((ALSoundState *) (base + (arg0->target * 0x30)))->state == 0) {
        ParamsRecAt8V1 p;
        p.f0 = 0;
        p.f4 = base + arg0->target * 0x30;
        func_00291034(&arg0->evtq, (s16 *)&p, 0);
    }
}
extern s32 func_00291034(void *, s16 *, s32);
