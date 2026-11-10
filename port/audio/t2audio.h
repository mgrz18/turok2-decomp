/* t2audio.h: the game's audio on the host (docs/port/AUDIO.md).
 *
 *   t2audio_bank.c  the sound bank (ROM 0x1E0000, two libaudio "B1" bank files whose pointers
 *                   are relative to the VM address 0x480000) as host structs with host
 *                   pointers in host byte order (read from the ROM image itself, so it does
 *                   not matter whether VM_Alloc/T2Asset return the ctl unit swapped or not),
 *                   and the sound data file (ROM 0xBE1814) byte-swapped in place unless it
 *                   already arrives in host order;
 *   t2audio_mgr.c   the audio manager (the SDK audiomgr.c pattern: amCreateAudioMgr, __amMain,
 *                   __amHandleFrameMsg, __amDmaNew, __amDMA) for the host: one synthesizer
 *                   frame per retrace, run through the ABI1 interpreter, handed to the AI;
 *   t2asp.c         the audio microcode (aspMain, ABI1) as a host interpreter.
 *
 * Plain C, no libultraship dependency. Off unless T2PortConfig.audio is set (--audio). */
#ifndef T2AUDIO_H
#define T2AUDIO_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- bank and sound data (t2audio_bank.c) ---- */

/* The ctl region's two bank files, converted once (NULL fields if there is no ROM). */
typedef struct T2AudioCtl {
    void *musicBank; /* ALBankFile * (the first bank file: CAudioWorld.seq.data) */
    void *sfxBank;   /* ALBankFile * (the second: CAudioWorld.bankFile) */
    uint32_t musicTbl, sfxTbl;
    int banks, instruments, sounds, waves;
} T2AudioCtl;
const T2AudioCtl *T2Audio_LoadCtl(void);

/* The sound data file (ROM 0xBE1814): swaps the copy at p (size bytes) to host order in place.
 * Returns the number of 16/32-bit words swapped, -1 if the layout is not recognized. */
long T2Audio_SwapSoundData(uint8_t *p, size_t size);

/* 1 if the sound data file at p is already in host order (r2's archive path swaps it) */
int T2Audio_SoundDataIsHostOrder(const uint8_t *p, size_t size);

/* VM_Alloc's host hook: fixes up a resident copy of a ROM region the audio code reads (a no-op
 * for a copy that already arrives in host order). */
void T2Audio_FixRegion(uint32_t romAddr, uint8_t *p, size_t size);

/* The wave table data: ROM offset (gAudioRomBase + wave base) -> host bytes. */
const uint8_t *T2Audio_TblBytes(uint32_t romAddr, uint32_t len);

/* ---- the microcode (t2asp.c) ---- */

typedef struct T2AspStats {
    uint64_t lists, commands, unknown;
    uint64_t perCmd[16];
} T2AspStats;
/* Runs an ABI1 command list (8-byte commands, words in host order as the game wrote them).
 * Addresses in the commands are host addresses (osVirtualToPhysical: the low 32 bits in low
 * memory mode) or segment-relative. */
void T2Asp_Run(const uint32_t *cmds, size_t count);
void T2Asp_GetStats(T2AspStats *out);

/* ---- output (t2audio_mgr.c / t2ultra.c) ---- */

/* The AI's queue of interleaved stereo s16 frames (host order) the game produced. Returns the
 * number of stereo frames copied into dst (at most max). */
size_t T2Audio_TakeSamples(int16_t *dst, size_t max);
size_t T2Audio_Queued(void);
uint32_t T2Audio_Rate(void);
/* --bench (docs/port/PERF.md): host time spent in the audio manager's frames so far, in ns:
 * the synthesizer (alAudioFrame: the game's sound player/sequencer and the command list) and the
 * ASP (T2Asp_Run: the mixer). Always counted (two clock reads per audio frame). */
void T2Audio_GetTimes(uint64_t *synthNs, uint64_t *aspNs);

typedef struct T2AudioStats {
    uint64_t frames;      /* synthesizer frames (alAudioFrame calls) */
    uint64_t samples;     /* stereo samples handed to the AI */
    uint64_t dropped;     /* stereo samples dropped (queue full: nobody drains it) */
    uint64_t dmaCalls;
    int peak;             /* max |sample| so far */
} T2AudioStats;
void T2Audio_GetStats(T2AudioStats *out);

/* Enabled by T2Port_Boot when T2PortConfig.audio is set. */
void T2Audio_Enable(int on);
int T2Audio_Enabled(void);

#ifdef __cplusplus
}
#endif

#endif
