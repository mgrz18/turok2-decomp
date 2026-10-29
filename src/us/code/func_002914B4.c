/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/sdk/libgultra_inferred/802B54A0_2.CPP, func_802B0554_de (libultra seqplayer.c __seqpVoiceHandler), with Turok 2's layouts. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern s32 func_00292530(void *arg0, s32 arg1, s32 arg2);
extern void func_00292B94(void *arg0, void *arg1);
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
struct Node_func_80239AF4_de;
typedef struct Node_func_80239AF4_de Node_func_80239AF4_de;
struct Node_func_80239AF4_de;
struct Node_func_80239AF4_de {
    struct Node_func_80239AF4_de *next;
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
struct Shape_typemap_71;
struct Shape_typemap_71 {
    unsigned char padding_0[28];
    int field_1C;
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
struct List802B663C;
typedef struct List802B663C List802B663C;
typedef signed int ( *Lane_ALOscUpdate)(void *, float *);
struct List802B663C;
struct List802B663C {
    char pad[0x64];
    Node_func_80239AF4_de *head;
    Node_func_80239AF4_de *tail;
    Node_func_80239AF4_de *free;
};
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
struct ALSeqMarker;
struct ALSeqMarker {
    u8 *curPtr;
    s32 lastTicks;
    s32 curTicks;
    s16 lastStatus;
};
struct ALSeqMarker;
typedef struct ALSeqMarker ALSeqMarker;
struct ALEvent;
extern void func_002924CC(List802B663C *arg0, void *arg1);
extern void func_00292C84(void *arg0);
extern void func_0029B020(const char *arg0, const char *arg1, s32 arg2);
struct ALSequencePlayer {
    ALSeqPlayer base;
    ALSeqMarker *loopStart;
    ALSeqMarker *loopEnd;
    s32 loopCount;
};
struct ALSequenceLoopEvent {
    ALSeqMarker *start;
    ALSeqMarker *end;
    s32 count;
};
extern char D_800AA380[];
extern char D_800AA384[];
extern void func_002919F0(ALSeqPlayer *);
extern void func_00291BF4(ALSeqPlayer *, ALEvent *);
extern void func_00292D4C(ALSeqPlayer *, ALEvent *);
extern void func_00292DB0(ALSeqPlayer *, f32);
extern void func_00291034(ALEventQueue *, ALEvent *, ALMicroTime);
extern ALMicroTime func_00290F9C(ALEventQueue *, ALEvent *);
extern void func_002911B8(ALEventQueue *, s16);
extern void alSynStopVoice(struct Shape_typemap_71 *, ALVoice_s *);
extern void func_00294160(void *, ALVoice_s *);
extern void alSynSetVol(void *, ALVoice_s *, s16, ALMicroTime);
extern void func_00294340(void *, ALVoice_s *, f32);
extern s16 func_00292774(ALVoiceState *, ALSeqPlayer *);
extern ALMicroTime func_002927E8(ALVoiceState *, ALMicroTime);
extern void func_00292A74(ALSeqPlayer *, ALVoice_s *, ALMicroTime);
extern void func_00292844(ALSeqPlayer *, void *);
ALMicroTime func_002914B4(void *node)
{
    struct ALSequencePlayer *seqp = node;
    ALEvent evt;
    ALVoice_s *voice;
    ALMicroTime delta;
    ALVoiceState *vs;
    void *oscState;
    f32 oscValue;
    u8 chan;
    u32 tremelo;
    u8 *tremeloSlot;
    do {
        switch (seqp->base.nextEvent.type) {
        case 0:
            func_002919F0(&seqp->base);
            break;
        case 9:
            evt.type = 9;
            func_00291034(&seqp->base.evtq, (ALEvent *)&evt, seqp->base.frameTime);
            break;
        case 5:
            voice = seqp->base.nextEvent.msg.note.voice;
            alSynStopVoice(seqp->base.drvr, voice);
            func_00294160(seqp->base.drvr, voice);
            vs = (ALVoiceState *)voice->clientPrivate;
            if (vs->flags)
                func_00292B94(&seqp->base, vs);
            func_002924CC(&seqp->base, voice);
            break;
        case 6:
            voice = seqp->base.nextEvent.msg.vol.voice;
            vs = (ALVoiceState *)voice->clientPrivate;
            if (vs->envPhase == 0)
                vs->envPhase = 1;
            delta = seqp->base.nextEvent.msg.vol.delta;
            vs->envGain = seqp->base.nextEvent.msg.vol.vol;
            vs->envEndTime = seqp->base.curTime + delta;
            alSynSetVol(seqp->base.drvr, voice, func_00292774(vs, &seqp->base), delta);
            break;
        case 22:
            vs = seqp->base.nextEvent.msg.osc.vs;
            oscState = seqp->base.nextEvent.msg.osc.oscState;
            delta = (*seqp->base.updateOsc)(oscState, &oscValue);
            tremeloSlot = &vs->tremelo;
            tremelo = (u32)oscValue;
            *tremeloSlot = tremelo;
            alSynSetVol(seqp->base.drvr, &vs->voice, func_00292774(vs, &seqp->base),
                          func_002927E8(vs, seqp->base.curTime));
            evt.type = 22;
            evt.msg.osc.vs = vs;
            evt.msg.osc.oscState = oscState;
            func_00291034(&seqp->base.evtq, &evt, delta);
            break;
        case 23:
            vs = seqp->base.nextEvent.msg.osc.vs;
            oscState = seqp->base.nextEvent.msg.osc.oscState;
            chan = seqp->base.nextEvent.msg.osc.chan;
            delta = (*seqp->base.updateOsc)(oscState, &oscValue);
            vs->vibrato = oscValue;
            func_00294340(seqp->base.drvr, &vs->voice,
                          vs->pitch * vs->vibrato * seqp->base.chanState[chan].pitchBend);
            evt.type = 23;
            evt.msg.osc.vs = vs;
            evt.msg.osc.oscState = oscState;
            evt.msg.osc.chan = chan;
            func_00291034(&seqp->base.evtq, &evt, delta);
            break;
        case 2:
            func_00291BF4(&seqp->base, &seqp->base.nextEvent);
            break;
        case 7:
            func_00292D4C(&seqp->base, &seqp->base.nextEvent);
            break;
        case 15:
            if (seqp->base.state != 1) {
                seqp->base.state = 1;
                func_00292C84(&seqp->base);
            }
            break;
        case 16:
            if (seqp->base.state == 2) {
                for (vs = seqp->base.vAllocHead; vs != 0; vs = seqp->base.vAllocHead) {
                    alSynStopVoice(seqp->base.drvr, &vs->voice);
                    func_00294160(seqp->base.drvr, &vs->voice);
                    if (vs->flags)
                        func_00292B94(&seqp->base, vs);
                    func_002924CC(&seqp->base, &vs->voice);
                }
                seqp->base.curTime = 0;
                seqp->base.state = 0;
            }
            break;
        case 17:
            if (seqp->base.state == 1) {
                func_002911B8(&seqp->base.evtq, 0);
                func_002911B8(&seqp->base.evtq, 2);
                for (vs = seqp->base.vAllocHead; vs != 0; vs = vs->next) {
                    if ((u8)func_00292530(&seqp->base, &vs->voice, 50000))
                        func_00292A74(&seqp->base, &vs->voice, 50000);
                }
                seqp->base.state = 2;
                evt.type = 16;
                func_00291034(&seqp->base.evtq, &evt, 0x7fffffff);
            }
            break;
        case 10:
            seqp->base.vol = seqp->base.nextEvent.msg.spvol.type;
            for (vs = seqp->base.vAllocHead; vs != 0; vs = vs->next) {
                alSynSetVol(seqp->base.drvr, &vs->voice, func_00292774(vs, &seqp->base),
                              func_002927E8(vs, seqp->base.curTime));
            }
            break;
        case 11:
            seqp->loopStart = ((struct ALSequenceLoopEvent *)&seqp->base.nextEvent.msg)->start;
            seqp->loopEnd = ((struct ALSequenceLoopEvent *)&seqp->base.nextEvent.msg)->end;
            seqp->loopCount = ((struct ALSequenceLoopEvent *)&seqp->base.nextEvent.msg)->count;
            break;
        case 12:
            chan = seqp->base.nextEvent.msg.sppriority.available;
            seqp->base.chanState[chan].priority = seqp->base.nextEvent.msg.sppriority.slot;
            break;
        case 13:
            if (seqp->base.state == 1)
                func_0029B020(D_800AA380, D_800AA384, 296);
            seqp->base.target = seqp->base.nextEvent.msg.spseq.unk0;
            func_00292DB0(&seqp->base, 500000.0f);
            if (seqp->base.bank)
                func_00292844(&seqp->base, seqp->base.bank);
            break;
        case 14:
            if (seqp->base.state != 0)
                func_0029B020(D_800AA380, D_800AA384, 305);
            seqp->base.bank = seqp->base.nextEvent.msg.spbank.unk0;
            func_00292844(&seqp->base, seqp->base.bank);
            break;
        case 4:
        case 3:
        case 1:
            func_0029B020(D_800AA380, D_800AA384, 315);
            break;
        }
        seqp->base.nextDelta = func_00290F9C(&seqp->base.evtq, &seqp->base.nextEvent);
    } while (seqp->base.nextDelta == 0);
    seqp->base.curTime += seqp->base.nextDelta;
    return seqp->base.nextDelta;
}
