/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra_inferred/802B54A0_3.CPP, func_802B0A90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
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
struct ALBank_s;
typedef struct ALBank_s ALBank_s;
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
struct ALInstrument;
typedef struct ALInstrument ALInstrument;
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
struct ALInstrument;
struct ALInstrument {
    u8 volume;
    u8 pan;
    u8 priority;
    u8 flags;
    u8 tremType;
    u8 tremRate;
    u8 tremDepth;
    u8 tremDelay;
    u8 vibType;
    u8 vibRate;
    u8 vibDepth;
    u8 vibDelay;
    s16 bendRange;
    s16 soundCount;
    ALSound_s *soundArray[1];
};
struct ALChanState10;
struct ALChanState10 {
    ALInstrument *instrument;
    s16 bendRange;
    u8 fxId;
    u8 pan;
    u8 priority;
    u8 vol;
    u8 fxmix;
    u8 sustain;
    f32 pitchBend;
};
struct ALSeqPlayer;
typedef struct ALSeqPlayer ALSeqPlayer;
struct ALSeqPlayer;
struct ALSeqPlayer;
struct ALBank_s;
struct ALBank_s {
    s16 instCount;
    u8 flags;
    u8 pad;
    s32 sampleRate;
    ALInstrument *percussion;
    ALInstrument *instArray[1];
};
struct ALVoiceState;
typedef struct ALVoiceState ALVoiceState;
struct ALSeqPlayer;
typedef signed int ( *Lane_ALOscUpdate)(void *, float *);
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
typedef signed int ( *Lane_ALOscInit)(void * *, float *, unsigned char, unsigned char, unsigned char, unsigned char);
typedef void ( *Lane_ALOscStop)(void *);
struct ALSeqPlayer;
struct ALVoiceState;
struct ALVoiceState;
typedef int ALMicroTime;
struct ALChanState10;
typedef struct ALChanState10 ALChanState10;
struct ALNoteEvent;
struct ALOscEvent;
struct ALVolumeEvent;
struct ALNoteEvent;
struct ALVoice_s;
struct ALEvent;
struct ALSeqPlayer;
struct ALSeqPlayer {
    ALPlayer_s node;
    void *drvr;
    void *target;
    ALMicroTime curTime;
    ALBank_s *bank;
    s32 uspt;
    s32 nextDelta;
    s32 state;
    u16 chanMask;
    s16 vol;
    u8 maxChannels;
    u8 debugFlags;
    ALEvent nextEvent;
    ALEventQueue evtq;
    ALMicroTime frameTime;
    ALChanState10 *chanState;
    ALVoiceState *vAllocHead;
    ALVoiceState *vAllocTail;
    ALVoiceState *vFreeList;
    Lane_ALOscInit initOsc;
    Lane_ALOscUpdate updateOsc;
    Lane_ALOscStop stopOsc;
};
struct ALEndEvent;
struct ALTempoEvent;
struct ALEvent;
struct ALSeq_s;
typedef struct ALSeq_s ALSeq_s;
struct ALSeqMarker;
struct ALSeqMarker {
    u8 *curPtr;
    s32 lastTicks;
    s32 curTicks;
    s16 lastStatus;
};
struct ALSeq_s;
struct ALSeq_s {
    u8 *base;
    u8 *trackStart;
    u8 *curPtr;
    s32 lastTicks;
    s32 len;
    f32 qnpt;
    s16 division;
    s16 lastStatus;
};
struct ALSeqMarker;
typedef struct ALSeqMarker ALSeqMarker;
struct ALEvent;
struct ALSeq_s;
struct ALSeq_s;
struct ALSequencePlayer {
    ALSeqPlayer base;
    ALSeqMarker *loopStart;
    ALSeqMarker *loopEnd;
    s32 loopCount;
};
extern char D_800AA380[];
extern char D_800AA384[];
extern void func_00292DE0(struct ALSeq_s *, ALEvent *);
extern void func_00291BF4(ALSeqPlayer *, ALEvent *);
extern void func_00292D4C(ALSeqPlayer *, ALEvent *);
extern u8 func_00293254(struct ALSeq_s *, s32 *);
extern int func_00293028(struct ALSeq_s *arg0);
extern void func_0029321C(struct ALSeq_s *, ALSeqMarker *);
extern void func_00291034(ALEventQueue *, ALEvent *, ALMicroTime);
static __inline__ void postNextSeqEvent(struct ALSequencePlayer *seqp) {
    ALEvent evt;
    s32 deltaTicks;
    struct ALSeq_s *seq = seqp->base.target;
    if ((seqp->base.state != 1) || (seq == 0))
        return;
    if (!func_00293254(seq, &deltaTicks))
        return;
    if (seqp->loopCount) {
        if (func_00293028(seq) + deltaTicks >= seqp->loopEnd->curTicks) {
            func_0029321C(seq, seqp->loopStart);
            if (seqp->loopCount != -1)
                seqp->loopCount--;
        }
    }
    evt.type = 0;
    func_00291034(&seqp->base.evtq, &evt, deltaTicks * seqp->base.uspt);
}
extern void func_0029B020(const char *arg0, const char *arg1, s32 arg2);
extern char D_800AA380[];
extern char D_800AA384[];
extern void func_00291034(ALEventQueue *, ALEvent *, ALMicroTime);
void func_002919F0(struct ALSequencePlayer *seqp)
{
    ALEvent evt;
    if (seqp->base.target == 0)
        return;
    func_00292DE0(seqp->base.target, &evt);
    switch (evt.type) {
    case 1:
        func_00291BF4(&seqp->base, &evt);
        postNextSeqEvent(seqp);
        break;
    case 3:
        func_00292D4C(&seqp->base, &evt);
        postNextSeqEvent(seqp);
        break;
    case 4:
        seqp->base.state = 2;
        evt.type = 16;
        func_00291034(&seqp->base.evtq, &evt, 0x7fffffff);
        break;
    default:
        func_0029B020(D_800AA380, D_800AA384, 412);
    }
}
void func_00291BF4(ALSeqPlayer *seqp, ALEvent *event);
