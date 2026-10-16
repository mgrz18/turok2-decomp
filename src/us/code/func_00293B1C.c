/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_6.c, func_802B2BBC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
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
struct ALEndEvent;
struct ALTempoEvent;
struct ALEvent;
union ALSndpEvent;
union ALSndpEvent {
    ALEvent msg;
    struct {
        s16 type;
        void *state;
    } common;
};
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
union ALSndpEvent;
struct ALSndPlayer;
typedef struct ALSndPlayer ALSndPlayer;
union ALSndpEvent;
typedef union ALSndpEvent ALSndpEvent;
extern void func_002936E0(struct ALSndPlayer * arg0, union ALSndpEvent * arg1);
extern s32 func_00293B1C(void *node);
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
typedef int ALMicroTime;
struct ALNoteEvent;
struct ALOscEvent;
struct ALVolumeEvent;
struct ALNoteEvent;
struct ALVoice_s;
struct ALEvent;
extern void func_00291034(ALEventQueue *, ALEvent *, ALMicroTime);
struct ALEvent;
extern s32 func_00290F9C(ALEventQueue *, ALEvent *);
s32 func_00293B1C(void *node)
{
    ALSndPlayer *sndp = (ALSndPlayer *)node;
    ALSndpEvent evt;
    ALSndpEvent *next = (ALSndpEvent *)&sndp->nextEvent;
    do {
        switch (sndp->nextEvent.type) {
            case 5:
                evt.common.type = 5;
                func_00291034(&sndp->evtq, (ALEvent *)&evt, sndp->frameTime);
                break;
            default:
                func_002936E0(sndp, next);
                break;
        }
        sndp->nextDelta = func_00290F9C(&sndp->evtq, &sndp->nextEvent);
    } while (sndp->nextDelta == 0);
    sndp->curTime += sndp->nextDelta;
    return sndp->nextDelta;
}
