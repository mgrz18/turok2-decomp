/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_5.c, func_802B2650_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef int (*ALVoiceHandler)(void *);
struct ALPlayer_s14;
struct ALPlayer_s14 {
    struct ALPlayer_s14 *next;
    void *clientData;
    ALVoiceHandler handler;
    s32 callTime;
    s32 samplesLeft;
};
struct ALPlayer_s14;
typedef struct ALPlayer_s14 ALPlayer_s14;
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
struct ALHeap;
typedef struct ALHeap ALHeap;
struct InventorySlot;
struct InventorySlot {
    u8 available;
    u8 slot;
};
struct InventorySlot;
typedef struct InventorySlot InventorySlot;
struct ALHeap;
struct ALHeap {
    u8 *base;
    u8 *cur;
    s32 len;
    s32 count;
};
struct ALMIDIEvent;
struct ALMIDIEvent {
    s32 ticks;
    u8 status;
    u8 byte1;
    u8 byte2;
    u32 duration;
};
struct ALEventListItem;
typedef struct ALEventListItem ALEventListItem;
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
struct ALEventListItem {
    ListLink node;
    s32 delta;
    ALEvent evt;
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
struct ALNoteEvent;
struct ALOscEvent;
struct ALVolumeEvent;
struct ALNoteEvent;
struct ALVoice_s;
struct ALEvent;
extern int func_00293B1C(void * arg0);
struct ALEvent;
struct ALSynth_func_802B2650_de;
struct ALSynth_func_802B2650_de {
    ALPlayer_s14 *head;
};
struct ALSndPlayer_func_802B2650_de;
struct ALSynth_func_802B2650_de;
struct ALSndPlayer_func_802B2650_de {
    ALPlayer_s14 node;
    ALEventQueue evtq;
    ALEvent nextEvent;
    struct ALSynth_func_802B2650_de *drvr;
    s32 target;
    void *sndState;
    s32 maxSounds;
    s32 frameTime;
    s32 nextDelta;
    s32 curTime;
};
struct ALGlobals_func_802B2650_de;
typedef struct ALGlobals_func_802B2650_de ALGlobals_func_802B2650_de;
struct ALSndpConfig;
struct ALSndpConfig {
    s32 maxSounds;
    s32 maxEvents;
    ALHeap *heap;
};
struct ALSoundState;
typedef struct ALSoundState ALSoundState;
struct ALSndPlayer_func_802B2650_de;
typedef struct ALSndPlayer_func_802B2650_de ALSndPlayer_func_802B2650_de;
struct ALSndpConfig;
typedef struct ALSndpConfig ALSndpConfig;
struct ALSynth_func_802B2650_de;
typedef struct ALSynth_func_802B2650_de ALSynth_func_802B2650_de;
struct ALGlobals_func_802B2650_de;
struct ALGlobals_func_802B2650_de {
    ALSynth_func_802B2650_de drvr;
};
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
extern ALGlobals_func_802B2650_de *D_800B7760;
extern void *func_002912A0(u8 *, s32, ALHeap *, s32, s32);
extern void func_00290F20(ALEventQueue *, ALEventListItem *, s32);
extern void func_00293F10(ALSynth_func_802B2650_de *, ALPlayer_s14 *);
extern void func_00291034(ALEventQueue *, ALEvent *, s32);
extern s32 func_00290F9C(ALEventQueue *, ALEvent *);
extern s32 func_00293B1C(void *);
void func_002935B0(ALSndPlayer_func_802B2650_de *sndp, ALSndpConfig *c)
{
    u8 *ptr;
    ALEvent evt;
    ALSoundState *sState;
    u32 i;
    sndp->maxSounds = c->maxSounds;
    sndp->target = -1;
    sndp->frameTime = 16000;
    sState = (ALSoundState *)func_002912A0(0, 0, c->heap, 1, c->maxSounds * sizeof(ALSoundState));
    sndp->sndState = sState;
    for (i = 0; i < c->maxSounds; i++)
        sState[i].sound = 0;
    ptr = func_002912A0(0, 0, c->heap, 1, c->maxEvents * sizeof(ALEventListItem));
    func_00290F20(&sndp->evtq, (ALEventListItem *)ptr, c->maxEvents);
    sndp->drvr = &D_800B7760->drvr;
    sndp->node.next = 0;
    sndp->node.handler = func_00293B1C;
    sndp->node.clientData = sndp;
    func_00293F10(sndp->drvr, &sndp->node);
    evt.type = 5;
    func_00291034(&sndp->evtq, (ALEvent *)&evt, sndp->frameTime);
    sndp->nextDelta = func_00290F9C(&sndp->evtq, &sndp->nextEvent);
}
