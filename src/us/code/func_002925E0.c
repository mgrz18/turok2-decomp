/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/seqplayer.c, __lookupVoice.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct {
    s32 ticks;
    u8 status;
    u8 byte1;
    u8 byte2;
    u32 duration;
} ALMIDIEvent;
typedef struct {
    s32 ticks;
    u8 status;
    u8 type;
    u8 len;
    u8 byte1;
    u8 byte2;
    u8 byte3;
} ALTempoEvent;
typedef struct ALLink_s {
    struct ALLink_s *next;
    struct ALLink_s *prev;
} ALLink;
typedef struct {
    ALLink freeList;
    ALLink allocList;
    s32 eventCount;
} ALEventQueue;
typedef struct ALPlayer_s {
    struct ALPlayer_s *next;
    void *clientData;
    void *handler;
    s32 callTime;
    s32 samplesLeft;
} ALPlayer;
typedef struct {
    u8 *base;
    u8 *trackStart;
    u8 *curPtr;
    s32 lastTicks;
    u32 len;
    f32 qnpt;
    s16 division;
    u16 lastStatus;
} ALSeq;
typedef struct {
    u8 *curPtr;
    s32 lastTicks;
    s32 curTicks;
    u16 lastStatus;
} ALSeqMarker;
typedef struct {
    u8 pad00[0x0C];
    u8 samplePan;
    u8 sampleVolume;
} ALSound;
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *(*handler)(struct ALFilter_s *filter, s16 *outp, s32 outCount, s32 sampleOffset, void *cmdp);
    s32 (*setParam)(struct ALFilter_s *filter, s32 type, void *param);
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
struct ALVoice_s;
typedef struct ALPVoice_s {
    ALLink node;
    struct ALVoice_s *vvoice;
    ALFilter *channelKnob;
    u8 decoder[0x48];
    u8 resampler[0x34];
    u8 envmixer[0x4C];
    s32 offset;
} ALPVoice;
typedef struct ALVoice_s {
    ALLink node;
    ALPVoice *pvoice;
    void *table;
    void *clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
} ALVoice;
typedef struct ALVoiceState_s {
    struct ALVoiceState_s *next;
    ALVoice voice;
    ALSound *sound;
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
} ALVoiceState;
typedef struct {
    void *inst;
    s16 bendRange;
    u8 fxId;
    u8 pan;
    u8 priority;
    u8 vol;
    u8 fxmix;
    u8 sustain;
    f32 pitchBend;
} ALChanStateFull;
typedef struct {
    s16 type;
    union {
        ALMIDIEvent midi;
        ALTempoEvent tempo;
        s32 ticks;
        struct { s16 vol; } spvol;
        struct { void *seq; } spseq;
        struct { void *bank; } spbank;
        struct { ALVoice *voice; } voice;
        struct { ALVoiceState *state; void *oscState; u8 chan; } osc;
        struct { ALSeqMarker *loopStart; ALSeqMarker *loopEnd; s32 loopCount; } loop;
        struct { ALVoice *voice; s32 delta; u8 gain; } env;
        struct { u8 chan; u8 priority; } prio;
    } msg;
} ALEventFull;
typedef struct {
    ALPlayer node;
    void *drvr;
    ALSeq *target;
    s32 curTime;
    void *bank;
    s32 uspt;
    s32 nextDelta;
    s32 state;
    u16 chanMask;
    s16 vol;
    u8 maxChannels;
    u8 debugFlags;
    u8 pad36[2];
    ALEventFull nextEvent;
    ALEventQueue evtq;
    s32 frameTime;
    ALChanStateFull *chanState;
    ALVoiceState *vAllocHead;
    ALVoiceState *vAllocTail;
    ALVoiceState *vFreeList;
    void *initOsc;
    void *updateOsc;
    void *stopOsc;
    ALSeqMarker *loopStart;
    ALSeqMarker *loopEnd;
    s32 loopCount;
} ALSeqPlayerFull;
ALVoiceState *func_002925E0(ALSeqPlayerFull *seqp, u8 key, u8 chan);
ALVoiceState *func_002925E0(ALSeqPlayerFull *seqp, u8 key, u8 chan) {
    ALVoiceState *vs;
    for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
        if (vs->key == key && vs->channel == chan && vs->phase != 3 && vs->phase != 4) {
            return vs;
        }
    }
    return 0;
}
