/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_11.c, func_802AEAB0_de.
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
extern char func_0028FB80;
struct ALEvent;
struct InitParams;
struct InitParams {
    s32 count38;
    s32 count1C;
    u8 kind;
    u8 pad09[3];
    s32 context;
    s32 value10;
    s32 value14;
    s32 value18;
};
struct InitParams;
typedef struct InitParams InitParams;
extern void func_002928F0(void *);
extern void *D_800B7760;
extern s32 func_002912A0(s32, s32, void *, s32, s32);
extern void func_00290F20(void *, void *, s32);
extern void func_00293F10(s32, void *);
void func_0028FA10(void *arg0, InitParams *arg1) {
    ALSeqPlayer *obj = arg0;
    InitParams *params = arg1;
    s32 context = params->context;
    s32 global = D_800B7760;
    s32 allocation;
    s32 i;
    obj->chanMask = 0xFF;
    obj->uspt = 0x1E8;
    obj->vol = 0x7FFF;
    obj->bank = 0;
    obj->target = 0;
    obj->nextDelta = 0;
    obj->state = 0;
    obj->frameTime = 0x3E80;
    obj->curTime = 0;
    obj->drvr = global;
    obj->initOsc = params->value10;
    obj->updateOsc = params->value14;
    obj->stopOsc = params->value18;
    obj->nextEvent.type = 9;
    obj->maxChannels = params->kind;
    obj->chanState = func_002912A0(0, 0, (void *)context, params->kind, 0x10);
    func_002928F0(obj);
    allocation = func_002912A0(0, 0, (void *)context, params->count38, 0x38);
    obj->vFreeList = 0;
    i = 0;
    if (params->count38 > 0) {
        {
            ALVoiceState *records = (ALVoiceState *)allocation;
            do {
                records->next = obj->vFreeList;
                obj->vFreeList = records;
                i++;
                records++;
            } while (i < params->count38);
        }
    }
    obj->vAllocHead = 0;
    obj->vAllocTail = 0;
    allocation = func_002912A0(0, 0, (void *)context, params->count1C, 0x1C);
    func_00290F20(&obj->evtq, (void *)allocation, params->count1C);
    {
        ALPlayer_s *node = &obj->node;
        node->next = 0;
        node->handler = &func_0028FB80;
        node->clientData = node;
        func_00293F10(D_800B7760, node);
    }
}
