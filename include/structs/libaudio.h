#ifndef STRUCTS_LIBAUDIO_H
#define STRUCTS_LIBAUDIO_H

/* libaudio types (PR/libaudio.h, PR/abi.h and the synthesizer, sequence
 * player and sound player internals of synthInternals.h, seqp.h and sndp.h),
 * with libultra 2.0I's names and layouts (decompals/ultralib, the version the
 * game's libaudio code matches). The sound manager (structs/sound.h), the
 * libaudio code itself and the code imported from the Rage Wars decomps
 * (rw/junk_types.h, rw/abu_types.h) share them.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

/* ---- PR/abi.h: RSP audio commands ---- */

typedef struct {
    unsigned int cmd:8;
    unsigned int flags:8;
    unsigned int gain:16;
    unsigned int addr;
} Aadpcm;

typedef struct {
    unsigned int cmd:8;
    unsigned int flags:8;
    unsigned int gain:16;
    unsigned int addr;
} Apolef;

typedef struct {
    unsigned int cmd:8;
    unsigned int flags:8;
    unsigned int pad1:16;
    unsigned int addr;
} Aenvelope;

typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:8;
    unsigned int dmem:16;
    unsigned int pad2:16;
    unsigned int count:16;
} Aclearbuff;

typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:8;
    unsigned int pad2:16;
    unsigned int inL:16;
    unsigned int inR:16;
} Ainterleave;

typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:24;
    unsigned int addr;
} Aloadbuff;

typedef struct {
    unsigned int cmd:8;
    unsigned int flags:8;
    unsigned int pad1:16;
    unsigned int addr;
} Aenvmixer;

typedef struct {
    unsigned int cmd:8;
    unsigned int flags:8;
    unsigned int gain:16;
    unsigned int dmemi:16;
    unsigned int dmemo:16;
} Amixer;

typedef struct {
    unsigned int cmd:8;
    unsigned int flags:8;
    unsigned int pitch:16;
    unsigned int addr;
} Aresample;

typedef struct {
    unsigned int cmd:8;
    unsigned int flags:8;
    unsigned int pad1:16;
    unsigned int addr;
} Areverb;

typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:24;
    unsigned int addr;
} Asavebuff;

typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:24;
    unsigned int pad2:2;
    unsigned int number:4;
    unsigned int base:24;
} Asegment;

typedef struct {
    unsigned int cmd:8;
    unsigned int flags:8;
    unsigned int dmemin:16;
    unsigned int dmemout:16;
    unsigned int count:16;
} Asetbuff;

typedef struct {
    unsigned int cmd:8;
    unsigned int flags:8;
    unsigned int vol:16;
    unsigned int voltgt:16;
    unsigned int volrate:16;
} Asetvol;

typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:8;
    unsigned int dmemin:16;
    unsigned int dmemout:16;
    unsigned int count:16;
} Admemmove;

typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:8;
    unsigned int count:16;
    unsigned int addr;
} Aloadadpcm;

typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:8;
    unsigned int pad2:16;
    unsigned int addr;
} Asetloop;

typedef struct {
    unsigned int w0;
    unsigned int w1;
} Awords;

/* An RSP audio command: the synthesizer pulls commands from its main bus,
 * which pulls them from each of its sources (alMainBusPull, func_00296360). */
typedef union {
    Awords words;
    Aadpcm adpcm;
    Apolef polef;
    Aclearbuff clearbuff;
    Aenvelope envelope;
    Ainterleave interleave;
    Aloadbuff loadbuff;
    Aenvmixer envmixer;
    Aresample resample;
    Areverb reverb;
    Asavebuff savebuff;
    Asegment segment;
    Asetbuff setbuff;
    Asetvol setvol;
    Admemmove dmemmove;
    Aloadadpcm loadadpcm;
    Amixer mixer;
    Asetloop setloop;
    long long force_align;
} Acmd;

typedef short ADPCM_STATE[16];
typedef short POLEF_STATE[4];
typedef short RESAMPLE_STATE[16];
typedef short ENVMIX_STATE[40];

/* ---- PR/libaudio.h ---- */

typedef s32 ALMicroTime;
typedef u8 ALPan;
typedef u8 ALFxId;
typedef void *ALFxRef;
typedef s16 ALSndId;

typedef struct ALLink_s {
    /* 0x0 */ struct ALLink_s *next;
    /* 0x4 */ struct ALLink_s *prev;
} ALLink; /* size 0x8 */

/* libaudio's heap. */
typedef struct {
    /* 0x0 */ u8 *base;
    /* 0x4 */ u8 *cur;
    /* 0x8 */ s32 len;
    /* 0xC */ s32 count;
} ALHeap; /* size 0x10 */

typedef s32 (*ALDMAproc)(s32 addr, s32 len, void *state);
typedef ALDMAproc (*ALDMANew)(void *state);
typedef ALMicroTime (*ALVoiceHandler)(void *);

/* Wave tables and banks (the .ctl/.tbl files). */
typedef struct {
    /* 0x0 */ s32 order;
    /* 0x4 */ s32 npredictors;
    /* 0x8 */ s16 book[1];
} ALADPCMBook;

typedef struct {
    /* 0x0 */ u32 start;
    /* 0x4 */ u32 end;
    /* 0x8 */ u32 count;
    /* 0xC */ ADPCM_STATE state;
} ALADPCMloop; /* size 0x2C */

typedef struct {
    /* 0x0 */ u32 start;
    /* 0x4 */ u32 end;
    /* 0x8 */ u32 count;
} ALRawLoop; /* size 0xC */

typedef struct {
    /* 0x0 */ ALMicroTime attackTime;
    /* 0x4 */ ALMicroTime decayTime;
    /* 0x8 */ ALMicroTime releaseTime;
    /* 0xC */ u8 attackVolume;
    /* 0xD */ u8 decayVolume;
} ALEnvelope; /* size 0x10 */

typedef struct {
    /* 0x0 */ u8 velocityMin;
    /* 0x1 */ u8 velocityMax;
    /* 0x2 */ u8 keyMin;
    /* 0x3 */ u8 keyMax;
    /* 0x4 */ u8 keyBase;
    /* 0x5 */ s8 detune;
} ALKeyMap; /* size 0x6 */

typedef struct {
    /* 0x0 */ ALADPCMloop *loop;
    /* 0x4 */ ALADPCMBook *book;
} ALADPCMWaveInfo;

typedef struct {
    /* 0x0 */ ALRawLoop *loop;
} ALRAWWaveInfo;

enum { AL_ADPCM_WAVE = 0, AL_RAW16_WAVE };

typedef struct ALWaveTable_s {
    /* 0x0 */ u8 *base;
    /* 0x4 */ s32 len;
    /* 0x8 */ u8 type;
    /* 0x9 */ u8 flags;
    /* 0xC */ union {
        ALADPCMWaveInfo adpcmWave;
        ALRAWWaveInfo rawWave;
    } waveInfo;
} ALWaveTable; /* size 0x14 */

typedef struct ALSound_s {
    /* 0x0 */ ALEnvelope *envelope;
    /* 0x4 */ ALKeyMap *keyMap;
    /* 0x8 */ ALWaveTable *wavetable;
    /* 0xC */ ALPan samplePan;
    /* 0xD */ u8 sampleVolume;
    /* 0xE */ u8 flags;
} ALSound; /* size 0x10 */

typedef struct {
    /* 0x00 */ u8 volume;
    /* 0x01 */ ALPan pan;
    /* 0x02 */ u8 priority;
    /* 0x03 */ u8 flags;
    /* 0x04 */ u8 tremType;
    /* 0x05 */ u8 tremRate;
    /* 0x06 */ u8 tremDepth;
    /* 0x07 */ u8 tremDelay;
    /* 0x08 */ u8 vibType;
    /* 0x09 */ u8 vibRate;
    /* 0x0A */ u8 vibDepth;
    /* 0x0B */ u8 vibDelay;
    /* 0x0C */ s16 bendRange;
    /* 0x0E */ s16 soundCount;
    /* 0x10 */ ALSound *soundArray[1];
} ALInstrument;

typedef struct ALBank_s {
    /* 0x0 */ s16 instCount;
    /* 0x2 */ u8 flags;
    /* 0x3 */ u8 pad;
    /* 0x4 */ s32 sampleRate;
    /* 0x8 */ ALInstrument *percussion;
    /* 0xC */ ALInstrument *instArray[1];
} ALBank;

typedef struct {
    /* 0x0 */ s16 revision;
    /* 0x2 */ s16 bankCount;
    /* 0x4 */ ALBank *bankArray[1];
} ALBankFile;

typedef struct {
    /* 0x0 */ u8 *offset;
    /* 0x4 */ s32 len;
} ALSeqData; /* size 0x8 */

typedef struct {
    /* 0x0 */ s16 revision;
    /* 0x2 */ s16 seqCount;
    /* 0x4 */ ALSeqData seqArray[1];
} ALSeqFile;

/* The synthesizer (alSynNew's config, alGlobals->drvr). */
typedef struct {
    /* 0x00 */ s32 maxVVoices;
    /* 0x04 */ s32 maxPVoices;
    /* 0x08 */ s32 maxUpdates;
    /* 0x0C */ s32 maxFXbusses;
    /* 0x10 */ void *dmaproc;
    /* 0x14 */ ALHeap *heap;
    /* 0x18 */ s32 outputRate;
    /* 0x1C */ ALFxId fxType;
    /* 0x20 */ s32 *params;
} ALSynConfig; /* size 0x24 */

/* A client of the synthesizer (a sequence or sound player): its handler is
 * called back every callTime microseconds. */
typedef struct ALPlayer_s {
    /* 0x00 */ struct ALPlayer_s *next;
    /* 0x04 */ void *clientData;
    /* 0x08 */ ALVoiceHandler handler;
    /* 0x0C */ ALMicroTime callTime;
    /* 0x10 */ s32 samplesLeft;
} ALPlayer; /* size 0x14 */

/* A virtual voice; the synthesizer binds it to a physical one (PVoice). */
typedef struct ALVoice_s {
    /* 0x00 */ ALLink node;
    /* 0x08 */ struct PVoice_s *pvoice;
    /* 0x0C */ ALWaveTable *table;
    /* 0x10 */ void *clientPrivate;
    /* 0x14 */ s16 state;
    /* 0x16 */ s16 priority;
    /* 0x18 */ s16 fxBus;
    /* 0x1A */ s16 unityPitch;
} ALVoice; /* size 0x1C */

typedef struct ALVoiceConfig_s {
    /* 0x0 */ s16 priority;
    /* 0x2 */ s16 fxBus;
    /* 0x4 */ u8 unityPitch;
} ALVoiceConfig; /* size 0x6 */

typedef struct {
    /* 0x00 */ ALPlayer *head;
    /* 0x04 */ ALLink pFreeList;
    /* 0x0C */ ALLink pAllocList;
    /* 0x14 */ ALLink pLameList;
    /* 0x1C */ s32 paramSamples;
    /* 0x20 */ s32 curSamples;
    /* 0x24 */ ALDMANew dma;
    /* 0x28 */ ALHeap *heap;
    /* 0x2C */ struct ALParam_s *paramList; /* free ALParams (__allocParam, __freeParam) */
    /* 0x30 */ struct ALMainBus_s *mainBus;
    /* 0x34 */ struct ALAuxBus_s *auxBus;
    /* 0x38 */ struct ALFilter_s *outputFilter;
    /* 0x3C */ s32 numPVoices;
    /* 0x40 */ s32 maxAuxBusses;
    /* 0x44 */ s32 outputRate;
    /* 0x48 */ s32 maxOutSamples;
} ALSynth; /* size 0x4C */

/* alGlobals is D_800B7760 (an ALGlobals *). */
typedef struct {
    /* 0x00 */ ALSynth drvr;
} ALGlobals; /* size 0x4C */

/* Sequence events. */
typedef struct {
    /* 0x0 */ u8 *curPtr;
    /* 0x4 */ s32 lastTicks;
    /* 0x8 */ s32 curTicks;
    /* 0xC */ s16 lastStatus;
} ALSeqMarker; /* size 0x10 */

typedef struct {
    /* 0x0 */ s32 ticks;
    /* 0x4 */ u8 status;
    /* 0x5 */ u8 byte1;
    /* 0x6 */ u8 byte2;
    /* 0x8 */ u32 duration;
} ALMIDIEvent; /* size 0xC */

typedef struct {
    /* 0x0 */ s32 ticks;
    /* 0x4 */ u8 status;
    /* 0x5 */ u8 type;
    /* 0x6 */ u8 len;
    /* 0x7 */ u8 byte1;
    /* 0x8 */ u8 byte2;
    /* 0x9 */ u8 byte3;
} ALTempoEvent; /* size 0xC */

typedef struct {
    /* 0x0 */ s32 ticks;
    /* 0x4 */ u8 status;
    /* 0x5 */ u8 type;
    /* 0x6 */ u8 len;
} ALEndEvent; /* size 0x8 */

typedef struct {
    /* 0x0 */ struct ALVoice_s *voice;
} ALNoteEvent;

typedef struct {
    /* 0x0 */ struct ALVoice_s *voice;
    /* 0x4 */ ALMicroTime delta;
    /* 0x8 */ u8 vol;
} ALVolumeEvent; /* size 0xC */

typedef struct {
    /* 0x0 */ s16 vol;
} ALSeqpVolEvent;

typedef struct {
    /* 0x0 */ ALSeqMarker *start;
    /* 0x4 */ ALSeqMarker *end;
    /* 0x8 */ s32 count;
} ALSeqpLoopEvent; /* size 0xC */

typedef struct {
    /* 0x0 */ u8 chan;
    /* 0x1 */ u8 priority;
} ALSeqpPriorityEvent;

typedef struct {
    /* 0x0 */ void *seq;
} ALSeqpSeqEvent;

typedef struct {
    /* 0x0 */ ALBank *bank;
} ALSeqpBankEvent;

typedef struct {
    /* 0x0 */ struct ALVoiceState_s *vs;
    /* 0x4 */ void *oscState;
    /* 0x8 */ u8 chan;
} ALOscEvent; /* size 0xC */

typedef struct {
    /* 0x0 */ s16 type;
    /* 0x4 */ union {
        ALMIDIEvent midi;
        ALTempoEvent tempo;
        ALEndEvent end;
        ALNoteEvent note;
        ALVolumeEvent vol;
        ALSeqpLoopEvent loop;
        ALSeqpVolEvent spvol;
        ALSeqpPriorityEvent sppriority;
        ALSeqpSeqEvent spseq;
        ALSeqpBankEvent spbank;
        ALOscEvent osc;
    } msg;
} ALEvent; /* size 0x10 */

typedef struct {
    /* 0x00 */ ALLink node;
    /* 0x08 */ ALMicroTime delta;
    /* 0x0C */ ALEvent evt;
} ALEventListItem; /* size 0x1C */

typedef struct {
    /* 0x00 */ ALLink freeList;
    /* 0x08 */ ALLink allocList;
    /* 0x10 */ s32 eventCount;
} ALEventQueue; /* size 0x14 */

/* The sequence players (seqp.h, cseqp.h). */
typedef struct ALVoiceState_s {
    /* 0x00 */ struct ALVoiceState_s *next;
    /* 0x04 */ ALVoice voice;
    /* 0x20 */ ALSound *sound;
    /* 0x24 */ ALMicroTime envEndTime;
    /* 0x28 */ f32 pitch;
    /* 0x2C */ f32 vibrato;
    /* 0x30 */ u8 envGain;
    /* 0x31 */ u8 channel;
    /* 0x32 */ u8 key;
    /* 0x33 */ u8 velocity;
    /* 0x34 */ u8 envPhase;
    /* 0x35 */ u8 phase;
    /* 0x36 */ u8 tremelo;
    /* 0x37 */ u8 flags; /* bit 0 tremelo, bit 1 vibrato */
} ALVoiceState; /* size 0x38 */

typedef struct {
    /* 0x0 */ ALInstrument *instrument;
    /* 0x4 */ s16 bendRange;
    /* 0x6 */ ALFxId fxId;
    /* 0x7 */ ALPan pan;
    /* 0x8 */ u8 priority;
    /* 0x9 */ u8 vol;
    /* 0xA */ u8 fxmix;
    /* 0xB */ u8 sustain;
    /* 0xC */ f32 pitchBend;
} ALChanState; /* size 0x10 */

typedef struct ALSeq_s {
    /* 0x00 */ u8 *base;
    /* 0x04 */ u8 *trackStart;
    /* 0x08 */ u8 *curPtr;
    /* 0x0C */ s32 lastTicks;
    /* 0x10 */ s32 len;
    /* 0x14 */ f32 qnpt;
    /* 0x18 */ s16 division;
    /* 0x1A */ s16 lastStatus;
} ALSeq; /* size 0x1C */

typedef struct {
    /* 0x00 */ u32 trackOffset[16];
    /* 0x40 */ u32 division;
} ALCMidiHdr; /* size 0x44 */

typedef struct ALCSeq_s {
    /* 0x00 */ ALCMidiHdr *base;
    /* 0x04 */ u32 validTracks;
    /* 0x08 */ f32 qnpt;
    /* 0x0C */ u32 lastTicks;
    /* 0x10 */ u32 lastDeltaTicks;
    /* 0x14 */ u32 deltaFlag;
    /* 0x18 */ u8 *curLoc[16];
    /* 0x58 */ u8 *curBUPtr[16];
    /* 0x98 */ u8 curBULen[16];
    /* 0xA8 */ u8 lastStatus[16];
    /* 0xB8 */ u32 evtDeltaTicks[16];
} ALCSeq; /* size 0xF8 */

typedef struct {
    /* 0x00 */ u32 validTracks;
    /* 0x04 */ s32 lastTicks;
    /* 0x08 */ u32 lastDeltaTicks;
    /* 0x0C */ u8 *curLoc[16];
    /* 0x4C */ u8 *curBUPtr[16];
    /* 0x8C */ u8 curBULen[16];
    /* 0x9C */ u8 lastStatus[16];
    /* 0xAC */ u32 evtDeltaTicks[16];
} ALCSeqMarker; /* size 0xEC */

typedef s32 (*ALOscInit)(void **oscState, f32 *initVal, u8 oscType, u8 oscRate, u8 oscDepth, u8 oscDelay);
typedef ALMicroTime (*ALOscUpdate)(void *oscState, f32 *updateVal);
typedef void (*ALOscStop)(void *oscState);

typedef struct {
    /* 0x00 */ s32 maxVoices;
    /* 0x04 */ s32 maxEvents;
    /* 0x08 */ u8 maxChannels;
    /* 0x09 */ u8 debugFlags;
    /* 0x0C */ ALHeap *heap;
    /* 0x10 */ void *initOsc;
    /* 0x14 */ void *updateOsc;
    /* 0x18 */ void *stopOsc;
} ALSeqpConfig; /* size 0x1C */

typedef struct {
    /* 0x00 */ ALPlayer node;
    /* 0x14 */ ALSynth *drvr;
    /* 0x18 */ ALSeq *target;
    /* 0x1C */ ALMicroTime curTime;
    /* 0x20 */ ALBank *bank;
    /* 0x24 */ s32 uspt;
    /* 0x28 */ s32 nextDelta;
    /* 0x2C */ s32 state;
    /* 0x30 */ u16 chanMask;
    /* 0x32 */ s16 vol;
    /* 0x34 */ u8 maxChannels;
    /* 0x35 */ u8 debugFlags;
    /* 0x38 */ ALEvent nextEvent;
    /* 0x48 */ ALEventQueue evtq;
    /* 0x5C */ ALMicroTime frameTime;
    /* 0x60 */ ALChanState *chanState;
    /* 0x64 */ ALVoiceState *vAllocHead;
    /* 0x68 */ ALVoiceState *vAllocTail;
    /* 0x6C */ ALVoiceState *vFreeList;
    /* 0x70 */ ALOscInit initOsc;
    /* 0x74 */ ALOscUpdate updateOsc;
    /* 0x78 */ ALOscStop stopOsc;
    /* 0x7C */ ALSeqMarker *loopStart;
    /* 0x80 */ ALSeqMarker *loopEnd;
    /* 0x84 */ s32 loopCount;
} ALSeqPlayer; /* size 0x88 */

/* The compact (.cmf) sequence player: ALSeqPlayer up to stopOsc. */
typedef struct {
    /* 0x00 */ ALPlayer node;
    /* 0x14 */ ALSynth *drvr;
    /* 0x18 */ ALCSeq *target;
    /* 0x1C */ ALMicroTime curTime;
    /* 0x20 */ ALBank *bank;
    /* 0x24 */ s32 uspt;
    /* 0x28 */ s32 nextDelta;
    /* 0x2C */ s32 state;
    /* 0x30 */ u16 chanMask;
    /* 0x32 */ s16 vol;
    /* 0x34 */ u8 maxChannels;
    /* 0x35 */ u8 debugFlags;
    /* 0x38 */ ALEvent nextEvent;
    /* 0x48 */ ALEventQueue evtq;
    /* 0x5C */ ALMicroTime frameTime;
    /* 0x60 */ ALChanState *chanState;
    /* 0x64 */ ALVoiceState *vAllocHead;
    /* 0x68 */ ALVoiceState *vAllocTail;
    /* 0x6C */ ALVoiceState *vFreeList;
    /* 0x70 */ ALOscInit initOsc;
    /* 0x74 */ ALOscUpdate updateOsc;
    /* 0x78 */ ALOscStop stopOsc;
} ALCSPlayer; /* size 0x7C */

/* The sound player (sndp.h). */
typedef struct {
    /* 0x0 */ s32 maxSounds;
    /* 0x4 */ s32 maxEvents;
    /* 0x8 */ ALHeap *heap;
} ALSndpConfig; /* size 0xC */

typedef struct {
    /* 0x00 */ ALPlayer node;
    /* 0x14 */ ALEventQueue evtq;
    /* 0x28 */ ALEvent nextEvent;
    /* 0x38 */ ALSynth *drvr;
    /* 0x3C */ s32 target;
    /* 0x40 */ void *sndState;
    /* 0x44 */ s32 maxSounds;
    /* 0x48 */ ALMicroTime frameTime;
    /* 0x4C */ ALMicroTime nextDelta;
    /* 0x50 */ ALMicroTime curTime;
} ALSndPlayer; /* size 0x54 */

typedef struct {
    /* 0x00 */ ALVoice voice;
    /* 0x1C */ ALSound *sound;
    /* 0x20 */ s16 priority;
    /* 0x24 */ f32 pitch;
    /* 0x28 */ s32 state;
    /* 0x2C */ s16 vol;
    /* 0x2E */ ALPan pan;
    /* 0x2F */ u8 fxMix;
} ALSoundState; /* size 0x30 */

typedef union {
    ALEvent msg;
    struct {
        s16 type;
        ALSoundState *state;
    } common;
    struct {
        s16 type;
        ALSoundState *state;
        s16 vol;
    } vol;
    struct {
        s16 type;
        ALSoundState *state;
        f32 pitch;
    } pitch;
    struct {
        s16 type;
        ALSoundState *state;
        ALPan pan;
    } pan;
    struct {
        s16 type;
        ALSoundState *state;
        u8 mix;
    } fx;
} ALSndpEvent;

/* ---- synthInternals.h: the synthesizer's filter chain ---- */

enum {
    AL_FILTER_FREE_VOICE,
    AL_FILTER_SET_SOURCE,
    AL_FILTER_ADD_SOURCE,
    AL_FILTER_ADD_UPDATE,
    AL_FILTER_RESET,
    AL_FILTER_SET_WAVETABLE,
    AL_FILTER_SET_DRAM,
    AL_FILTER_SET_PITCH,
    AL_FILTER_SET_UNITY_PITCH,
    AL_FILTER_START,
    AL_FILTER_SET_STATE,
    AL_FILTER_SET_VOLUME,
    AL_FILTER_SET_PAN,
    AL_FILTER_START_VOICE_ALT,
    AL_FILTER_START_VOICE,
    AL_FILTER_STOP_VOICE,
    AL_FILTER_SET_FXAMT
};

/* A parameter update queued on a filter (the free ones are alGlobals->drvr's
 * paramList). */
typedef struct ALParam_s {
    /* 0x00 */ struct ALParam_s *next;
    /* 0x04 */ s32 delta;
    /* 0x08 */ s16 type;
    /* 0x0C */ union {
        f32 f;
        s32 i;
    } data;
    /* 0x10 */ union {
        f32 f;
        s32 i;
    } moredata;
    /* 0x14 */ union {
        f32 f;
        s32 i;
    } stillmoredata;
    /* 0x18 */ union {
        f32 f;
        s32 i;
    } yetstillmoredata;
} ALParam; /* size 0x1C */

typedef struct {
    /* 0x00 */ struct ALParam_s *next;
    /* 0x04 */ s32 delta;
    /* 0x08 */ s16 type;
    /* 0x0A */ s16 unity;
    /* 0x0C */ f32 pitch;
    /* 0x10 */ s16 volume;
    /* 0x12 */ ALPan pan;
    /* 0x13 */ u8 fxMix;
    /* 0x14 */ s32 samples;
    /* 0x18 */ struct ALWaveTable_s *wave;
} ALStartParamAlt; /* size 0x1C */

typedef struct {
    /* 0x00 */ struct ALParam_s *next;
    /* 0x04 */ s32 delta;
    /* 0x08 */ s16 type;
    /* 0x0A */ s16 unity;
    /* 0x0C */ struct ALWaveTable_s *wave;
} ALStartParam; /* size 0x10 */

typedef struct {
    /* 0x00 */ struct ALParam_s *next;
    /* 0x04 */ s32 delta;
    /* 0x08 */ s16 type;
    /* 0x0C */ struct PVoice_s *pvoice;
} ALFreeParam; /* size 0x10 */

typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);
typedef s32 (*ALSetParam)(void *, s32, void *);

typedef struct ALFilter_s {
    /* 0x00 */ struct ALFilter_s *source;
    /* 0x04 */ ALCmdHandler handler;
    /* 0x08 */ ALSetParam setParam;
    /* 0x0C */ s16 inp;
    /* 0x0E */ s16 outp;
    /* 0x10 */ s32 type;
} ALFilter; /* size 0x14 */

/* The ADPCM / raw16 decoder. */
typedef struct {
    /* 0x00 */ ALFilter filter;
    /* 0x14 */ ADPCM_STATE *state;
    /* 0x18 */ ADPCM_STATE *lstate;
    /* 0x1C */ ALRawLoop loop;
    /* 0x28 */ struct ALWaveTable_s *table;
    /* 0x2C */ s32 bookSize;
    /* 0x30 */ ALDMAproc dma;
    /* 0x34 */ void *dmaState;
    /* 0x38 */ s32 sample;
    /* 0x3C */ s32 lastsam;
    /* 0x40 */ s32 first;
    /* 0x44 */ s32 memin;
} ALLoadFilter; /* size 0x48 */

typedef struct ALResampler_s {
    /* 0x00 */ ALFilter filter;
    /* 0x14 */ RESAMPLE_STATE *state;
    /* 0x18 */ f32 ratio;
    /* 0x1C */ s32 upitch;
    /* 0x20 */ f32 delta;
    /* 0x24 */ s32 first;
    /* 0x28 */ ALParam *ctrlList;
    /* 0x2C */ ALParam *ctrlTail;
    /* 0x30 */ s32 motion;
} ALResampler; /* size 0x34 */

typedef struct {
    /* 0x00 */ s16 fc;
    /* 0x02 */ s16 fgain;
    /* 0x08 */ union {
        s16 fccoef[16];
        s64 force_aligned;
    } fcvec;
    /* 0x28 */ POLEF_STATE *fstate;
    /* 0x2C */ s32 first;
} ALLowPass; /* size 0x30 */

typedef struct {
    /* 0x00 */ u32 input;
    /* 0x04 */ u32 output;
    /* 0x08 */ s16 ffcoef;
    /* 0x0A */ s16 fbcoef;
    /* 0x0C */ s16 gain;
    /* 0x10 */ f32 rsinc;
    /* 0x14 */ f32 rsval;
    /* 0x18 */ s32 rsdelta;
    /* 0x1C */ f32 rsgain;
    /* 0x20 */ ALLowPass *lp;
    /* 0x24 */ ALResampler *rs;
} ALDelay; /* size 0x28 */

typedef s32 (*ALSetFXParam)(void *, s32, void *);

/* The reverb (an aux bus effect). */
typedef struct {
    /* 0x00 */ struct ALFilter_s filter;
    /* 0x14 */ s16 *base;
    /* 0x18 */ s16 *input;
    /* 0x1C */ u32 length;
    /* 0x20 */ ALDelay *delay;
    /* 0x24 */ u8 section_count;
    /* 0x28 */ ALSetFXParam paramHdl;
} ALFx; /* size 0x2C */

typedef struct ALMainBus_s {
    /* 0x00 */ ALFilter filter;
    /* 0x14 */ s32 sourceCount;
    /* 0x18 */ s32 maxSources;
    /* 0x1C */ ALFilter **sources;
} ALMainBus; /* size 0x20 */

typedef struct ALAuxBus_s {
    /* 0x00 */ ALFilter filter;
    /* 0x14 */ s32 sourceCount;
    /* 0x18 */ s32 maxSources;
    /* 0x1C */ ALFilter **sources;
    /* 0x20 */ ALFx fx[1];
} ALAuxBus; /* size 0x4C */

typedef struct ALSave_s {
    /* 0x00 */ ALFilter filter;
    /* 0x14 */ s32 dramout;
    /* 0x18 */ s32 first;
} ALSave; /* size 0x1C */

/* The envelope / pan mixer. */
typedef struct ALEnvMixer_s {
    /* 0x00 */ ALFilter filter;
    /* 0x14 */ ENVMIX_STATE *state;
    /* 0x18 */ s16 pan;
    /* 0x1A */ s16 volume;
    /* 0x1C */ s16 cvolL;
    /* 0x1E */ s16 cvolR;
    /* 0x20 */ s16 dryamt;
    /* 0x22 */ s16 wetamt;
    /* 0x24 */ u16 lratl;
    /* 0x26 */ s16 lratm;
    /* 0x28 */ s16 ltgt;
    /* 0x2A */ u16 rratl;
    /* 0x2C */ s16 rratm;
    /* 0x2E */ s16 rtgt;
    /* 0x30 */ s32 delta;
    /* 0x34 */ s32 segEnd;
    /* 0x38 */ s32 first;
    /* 0x3C */ ALParam *ctrlList;
    /* 0x40 */ ALParam *ctrlTail;
    /* 0x44 */ ALFilter **sources;
    /* 0x48 */ s32 motion;
} ALEnvMixer; /* size 0x4C */

/* A physical voice: decoder -> resampler -> envelope mixer. */
typedef struct PVoice_s {
    /* 0x00 */ ALLink node;
    /* 0x08 */ struct ALVoice_s *vvoice;
    /* 0x0C */ ALFilter *channelKnob;
    /* 0x10 */ ALLoadFilter decoder;
    /* 0x58 */ ALResampler resampler;
    /* 0x8C */ ALEnvMixer envmixer;
    /* 0xD8 */ s32 offset;
} PVoice; /* size 0xDC */

#endif
