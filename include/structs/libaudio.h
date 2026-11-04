#ifndef STRUCTS_LIBAUDIO_H
#define STRUCTS_LIBAUDIO_H

/* libaudio types (PR/libaudio.h, PR/abi.h and the synthesizer internals of
 * synthInternals.h), as the SDK defines them. The sound manager
 * (structs/sound.h) and the libaudio code imported from the Rage Wars decomps
 * (rw/junk_types.h, rw/abu_types.h) share them. */

#include "common.h"

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
 * (alSynNew has the full layout). */
typedef struct {
    /* 0x00 */ ALFilter filter;
    /* 0x14 */ s32 sourceCount;
    /* 0x18 */ s32 maxSources;
    /* 0x1C */ ALFilter **sources;
} ALAuxBus;

#endif
