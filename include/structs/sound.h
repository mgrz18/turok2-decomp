#ifndef STRUCTS_SOUND_H
#define STRUCTS_SOUND_H

/* The sound manager and its voices.
 *
 * One SndMgr drives all sound effects (Rage Wars' audio context,
 * ContextRecAt2BC0 at D_8010C080 there). func_00270240 builds it: an audio
 * heap, the libaudio synthesizer and sound player, the sound bank, then the
 * voice queue, the music sequence, the 17-voice pool and the effects block.
 * Its CMutex is taken around every public call. A playing sound is a SndVoice
 * (0xCC bytes): its parameters block is what the sound player is fed
 * (alSndpSetPitch/FXMix/Pan/Vol, func_002706A0), followed by three envelopes
 * stepped every frame. Names come from Rage Wars (rw/junk_types.h's M04Emitter
 * / M04VP / M04Ctx, rw_abu's ContextRecAt2BC0 / Voice) and from what the code
 * does. */

#include "common.h"
#include "ultra_types.h"
#include "structs/game_object.h" /* CVector3 */

typedef struct SndMgr SndMgr;

/* libaudio's heap (PR/libaudio.h). */
typedef struct {
    /* 0x0 */ u8 *base;
    /* 0x4 */ u8 *cur;
    /* 0x8 */ s32 len;
    /* 0xC */ s32 count;
} ALHeap; /* size 0x10 */

/* libaudio's sound bank (PR/libaudio.h); ALSound is left opaque. */
typedef struct {
    /* 0x00 */ u8 volume;
    /* 0x01 */ u8 pan;
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
    /* 0x10 */ struct ALSound_s *soundArray[1];
} ALInstrument;

typedef struct {
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

/* An RSP audio command (PR/abi.h) and libaudio's filter chain
 * (synthInternals.h): the synthesizer pulls commands from its main bus,
 * which pulls them from each of its sources (alMainBusPull, func_00296360). */
typedef union {
    struct {
        u32 w0;
        u32 w1;
    } words;
    long long force_align;
} Acmd;

typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);

typedef struct ALFilter_s {
    /* 0x00 */ struct ALFilter_s *source;
    /* 0x04 */ ALCmdHandler handler;
    /* 0x08 */ void *setParam;
    /* 0x0C */ s16 inp;
    /* 0x0E */ s16 outp;
    /* 0x10 */ s32 type;
} ALFilter; /* size 0x14 */

typedef struct {
    /* 0x00 */ ALFilter filter;
    /* 0x14 */ s32 sourceCount;
    /* 0x18 */ s32 maxSources;
    /* 0x1C */ ALFilter **sources;
} ALMainBus; /* size 0x20 */

/* The aux bus starts like the main bus; its ALFx effect follows at 0x20
 * (func_002946D0 has the full layout). */
typedef struct {
    /* 0x00 */ ALFilter filter;
    /* 0x14 */ s32 sourceCount;
    /* 0x18 */ s32 maxSources;
    /* 0x1C */ ALFilter **sources;
} ALAuxBus;

/* Value at the voice's pitch slot: written as an int, read as a float. */
typedef union {
    s32 i;
    f32 f;
} SndIF32;

/* What the sound player is given for one voice (M04VP). */
typedef struct {
    /* 0x00 */ s32 frame;        /* SndMgr.frame when it started */
    /* 0x04 */ s32 w4;
    /* 0x08 */ s32 vol;        /* alSndpSetVol, times scale and the volume setting */
    /* 0x0C */ s32 wC;
    /* 0x10 */ s16 fxMix;      /* alSndpSetFXMix, plus SndMgr.fxMix */
    /* 0x12 */ u8 pad12[2];
    /* 0x14 */ SndIF32 pitch;  /* alSndpSetPitch */
    /* 0x18 */ s16 pan;        /* alSndpSetPan (its low byte) */
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ f32 scale;      /* volume scale */
    /* 0x20 */ f32 f20;
    /* 0x24 */ f32 f24;
    /* 0x28 */ s16 priority;
    /* 0x2A */ s16 channel;
    /* 0x2C */ s16 s2C;
    /* 0x2E */ u8 pad2E[6];
    /* 0x34 */ CVector3 pos;   /* world position of a 3D sound */
    /* 0x40 */ s32 state;
    /* 0x44 */ s8 distanceMode;
    /* 0x45 */ u8 pad45[3];
    /* 0x48 */ f32 distSq;     /* squared distance to the listener */
    /* 0x4C */ f32 prevDistSq;
} SndVoiceParams; /* size 0x50 */

/* One envelope segment (M04Rec12): after delay frames the value steps
 * toward target. */
typedef struct {
    /* 0x0 */ u16 level;
    /* 0x2 */ u16 unk2;
    /* 0x4 */ u16 target;
    /* 0x6 */ s16 frames;
    /* 0x8 */ s16 delay;
    /* 0xA */ u16 unkA;
} SndEnv; /* size 0xC */

/* A playing sound (M04Emitter; rw_abu's Voice). */
typedef struct SndVoice {
    /* 0x00 */ s32 index;      /* slot in the pool */
    /* 0x04 */ s32 sound;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 id;         /* the handle handed out for it */
    /* 0x10 */ SndVoiceParams vp;
    /* 0x60 */ SndEnv env;
    /* 0x6C */ f32 envValue;
    /* 0x70 */ f32 envStep;
    /* 0x74 */ SndEnv env2;
    /* 0x80 */ f32 env2Value;
    /* 0x84 */ f32 env2Step;
    /* 0x88 */ s32 hasEnv;
    /* 0x8C */ SndEnv env3;
    /* 0x98 */ f32 env3Value;
    /* 0x9C */ f32 env3Step;
    /* 0xA0 */ s32 pending;
    /* 0xA4 */ u32 flags;      /* 0x4 enveloped, 0x8 stop, 0x200 music volume, 0x400 positional */
    /* 0xA8 */ s32 soundId;
    /* 0xAC */ s32 stopping;
    /* 0xB0 */ SndMgr *mgr;
    /* 0xB4 */ s32 unkB4;
    /* 0xB8 */ f32 pitchScale;
    /* 0xBC */ s32 unkBC;
    /* 0xC0 */ s32 reset;
    /* 0xC4 */ s32 unkC4;
    /* 0xC8 */ f32 volumeScale;
} SndVoice; /* size 0xCC */

typedef struct SndLink {
    struct SndLink *prev;
    struct SndLink *next;
} SndLink;

typedef struct {
    /* 0x0 */ SndLink link;
    /* 0x8 */ SndVoice body;
} SndNode; /* size 0xD4 */

/* Requests waiting for a voice (initialised by func_002763F4). */
typedef struct {
    /* 0x0000 */ SndMgr *owner;
    /* 0x0004 */ SndNode active;
    /* 0x00D8 */ SndNode free;
    /* 0x01AC */ SndNode nodes[32];
} SndQueue; /* size 0x1C2C */

/* The music sequence (initialised by func_00274F34). */
typedef struct {
    /* 0x00 */ SndMgr *owner;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 *data;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ void *unk10;
    /* 0x14 */ void *csp;      /* compressed sequence player */
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ f32 vol;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ SndIF32 unk40;
} SndSeq; /* size 0x44 */

/* The voices (initialised by func_00273A94). */
typedef struct {
    /* 0x000 */ SndMgr *owner;
    /* 0x004 */ SndVoice voices[17];
} SndVoicePool; /* size 0xD90 */

/* Effects block (initialised by func_00276D98). */
typedef struct {
    /* 0x00 */ SndMgr *owner;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 sndId;
    /* 0x20 */ u8 pad20[0x20];
} SndFx; /* size 0x40 */

struct SndMgr {
    /* 0x0000 */ u8 pad0[0x24];
    /* 0x0024 */ s32 playerConfig[3];  /* ALSndpConfig: maxSounds, maxEvents, heap */
    /* 0x0030 */ u8 pad30[0x4C];
    /* 0x007C */ ALBank *bank;         /* bankFile->bankArray[0] */
    /* 0x0080 */ ALBankFile *bankFile;
    /* 0x0084 */ u8 player[0x54];      /* ALSndPlayer */
    /* 0x00D8 */ s32 soundState;
    /* 0x00DC */ s16 handles[16];
    /* 0x00FC */ u8 padFC[6];
    /* 0x0102 */ s16 pendingCue;
    /* 0x0104 */ s32 frame;
    /* 0x0108 */ s32 soundCounter;
    /* 0x010C */ s32 handle;           /* last handle handed out */
    /* 0x0110 */ CMutex lock;
    /* 0x0130 */ s32 lastVariant;
    /* 0x0134 */ s32 track;
    /* 0x0138 */ SndQueue queue;
    /* 0x1D64 */ SndSeq seq;
    /* 0x1DA8 */ ALHeap heap;
    /* 0x1DB8 */ SndVoicePool pool;
    /* 0x2B48 */ void *soundData;      /* the sound tree file (func_00270550) */
    /* 0x2B4C */ u32 *variants;
    /* 0x2B50 */ void *node2B50;
    /* 0x2B54 */ s32 rangeCount;
    /* 0x2B58 */ void *ranges;
    /* 0x2B5C */ s32 *weights;
    /* 0x2B60 */ void *table2B60;
    /* 0x2B64 */ s32 count2B64;
    /* 0x2B68 */ u8 pad2B68[8];
    /* 0x2B70 */ u8 *table2B70;        /* 0xC-byte entries */
    /* 0x2B74 */ u8 *table2B74;        /* 0xC-byte entries */
    /* 0x2B78 */ u8 *table2B78;
    /* 0x2B7C */ s32 count2B70;
    /* 0x2B80 */ s32 count2B74;
    /* 0x2B84 */ s32 count2B78;
    /* 0x2B88 */ u8 *cue;
    /* 0x2B8C */ s16 cueId;
    /* 0x2B8E */ u8 pad2B8E[2];
    /* 0x2B90 */ s32 cueArg;
    /* 0x2B94 */ s8 cueVolume;      /* also the default pan */
    /* 0x2B95 */ u8 pad2B95[3];
    /* 0x2B98 */ struct CCamera *listener; /* sounds are heard from its m_vPos, out to m_FarClip */
    /* 0x2B9C */ s32 fxMix;            /* added to each voice's FX mix */
    /* 0x2BA0 */ f32 sfxVolume;        /* option byte D_8011AAED / 255 */
    /* 0x2BA4 */ f32 volume2BA4;       /* option byte D_8011AAEC / 255 */
    /* 0x2BA8 */ f32 musicVolume;      /* option byte D_8011AAEE / 255; voices flagged 0x200 */
    /* 0x2BAC */ s32 unk2BAC;
    /* 0x2BB0 */ s32 unk2BB0;          /* option byte D_8011AAEB */
    /* 0x2BB4 */ s32 unk2BB4;
    /* 0x2BB8 */ s32 muted;
    /* 0x2BBC */ f32 unk2BBC;
    /* 0x2BC0 */ SndFx fx;
}; /* size 0x2C00 */

/* A sound effect descriptor (SndInfo in rw/junk_types.h), read through the
 * accessors at 0x276820..0x2768BC. */
typedef struct SoundEffectInfo {
    /* 0x00 */ s16 group;
    /* 0x02 */ s16 sample;
    /* 0x04 */ s8 priority;
    /* 0x05 */ s8 chance;
    /* 0x06 */ u16 flags;      /* low 14 bits flags, top 2 bits pan */
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s8 pitch;
    /* 0x0F */ s8 unkF;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 randomRange0;
    /* 0x14 */ s16 randomRange1;
} SoundEffectInfo; /* size 0x16 */

#endif
