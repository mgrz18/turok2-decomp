/* t2audio_test: the game's synthesizer on the host, without the engine (docs/port/AUDIO.md §6).
 *
 *   t2audio_test baserom.us.z64 [--wav out.wav] [--frames N] [--sfx K] [--song S]
 *   t2audio_test baserom.us.z64 --direct K out.raw [--wav out.wav]
 *
 * Boots t2os (T2Port_Boot with a test main thread instead of mainproc), gives the sound world
 * an audio heap (GfxPool_Alloc's job in the game), runs Sound__Initialize (the converted bank,
 * the swapped sound data file, the host audio manager), then:
 *   phase 1: sound effect K of the sfx bank through the game's sound player (alSndp*),
 *   phase 2: song S through the game's music code (Music_SetTrack -> SndSeq_Update),
 * each for a while, one synthesizer frame per retrace through the ABI1 interpreter. Checks
 * that each phase is non-silent and sane (peak, RMS, no stuck DC, few clipped samples) and
 * writes the whole output as a WAV (22050 Hz stereo) if asked. Exit status 0 = pass. */
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "ultra_types.h"
#include "structs/libaudio.h"
#include "structs/sound.h"
#include "protos/ultra.h"
#include "t2port.h"

#ifndef OS_MESG_BLOCK
#define OS_MESG_BLOCK 1
#endif
#include "t2audio.h"

extern CAudioWorld gAudioWorld;
extern sptr gAudioHeapBase; /* include/globals/frame.h: pointer-wide on the host */
extern s32 gAudioHeapSize;
extern s32 gAudioWorldReady;
void Sound__Initialize(void);
void Sound__Lock(void);
void Sound__Unlock(void);
s16 alSndpAllocate(ALSndPlayer *sndp, ALSound *sound);
void alSndpSetSound(ALSndPlayer *sndp, s16 id);
void alSndpPlay(ALSndPlayer *sndp);
void alSndpSetVol(ALSndPlayer *sndp, s16 vol);
void alSndpSetPan(ALSndPlayer *sndp, ALPan pan);
s32 alSndpGetState(ALSndPlayer *sndp);
void Music_SetTrack(s32 track);

#define AUDIO_HEAP_SIZE 0x180000

static int sSfx = 0, sSong = -1, sFrames = 240;
static volatile int sPhase; /* set by the platform loop; the test thread acts on changes */
static OSMesgQueue sTickQ;
static OSMesg sTickMsg[4];
static u8 sTimerKey[0x40]; /* an OSTimer-sized key (t2ultra uses its address only) */
static int sInitOk;

static void test_main(void *arg) {
    int phase = 0;
    OSMesg m;
    (void)arg;
    gAudioHeapSize = AUDIO_HEAP_SIZE;
    gAudioHeapBase = (sptr)t2_lowmem_alloc(AUDIO_HEAP_SIZE);
    Sound__Initialize();
    sInitOk = gAudioWorldReady;
    fprintf(stderr, "test: Sound__Initialize done (gAudioWorldReady %d, bank %p, %d instruments in bank 0)\n",
            (int)gAudioWorldReady, (void *)gAudioWorld.bank, gAudioWorld.bank ? gAudioWorld.bank->instCount : -1);
    osCreateMesgQueue(&sTickQ, sTickMsg, 4);
    osSetTimer((OSTimer *)sTimerKey, 781250ull, 781250ull, &sTickQ, (OSMesg)1);
    for (;;) {
        osRecvMesg(&sTickQ, &m, OS_MESG_BLOCK);
        if (sPhase == phase)
            continue;
        phase = sPhase;
        if (phase == 1 && gAudioWorld.bank != NULL) {
            ALInstrument *inst = gAudioWorld.bank->instArray[0];
            ALSound *snd = inst && sSfx < inst->soundCount ? inst->soundArray[sSfx] : NULL;
            s16 id;
            fprintf(stderr, "test: sfx %d: sound %p (wave len %d, type %d)\n", sSfx, (void *)snd,
                    snd && snd->wavetable ? (int)snd->wavetable->len : -1,
                    snd && snd->wavetable ? (int)snd->wavetable->type : -1);
            if (snd == NULL)
                continue;
            Sound__Lock();
            id = alSndpAllocate(&gAudioWorld.player, snd);
            fprintf(stderr, "test: alSndpAllocate -> %d\n", (int)id);
            if (id >= 0) {
                alSndpSetSound(&gAudioWorld.player, id);
                alSndpSetVol(&gAudioWorld.player, 0x7FFF);
                alSndpSetPan(&gAudioWorld.player, 64);
                alSndpPlay(&gAudioWorld.player);
            }
            Sound__Unlock();
        } else if (phase == 2 && sSong >= 0) {
            fprintf(stderr, "test: Music_SetTrack(%d)\n", sSong);
            Music_SetTrack(sSong);
        }
    }
}

typedef struct {
    double sumsq, sum;
    int peak, clipped;
    long n;
} Stats;

static void stats_add(Stats *s, const int16_t *buf, size_t frames) {
    size_t i;
    for (i = 0; i < frames * 2; i++) {
        int v = buf[i];
        int a = v < 0 ? -v : v;
        s->sumsq += (double)v * v;
        s->sum += v;
        if (a > s->peak)
            s->peak = a;
        if (a >= 32767)
            s->clipped++;
    }
    s->n += (long)frames * 2;
}

static int stats_check(const char *what, const Stats *s) {
    double rms = s->n ? sqrt(s->sumsq / (double)s->n) : 0, dc = s->n ? s->sum / (double)s->n : 0;
    double clip = s->n ? 100.0 * s->clipped / (double)s->n : 0;
    int ok = s->peak > 200 && rms > 20 && fabs(dc) < 0.5 * rms + 50 && clip < 1.0;
    printf("  %-6s %ld samples, peak %d, RMS %.1f, DC %.1f, clipped %.3f%%: %s\n", what, s->n, s->peak, rms, dc, clip,
           ok ? "ok" : "FAIL");
    return ok;
}

static void wav_write(const char *path, const int16_t *pcm, size_t frames, uint32_t rate) {
    FILE *f = fopen(path, "wb");
    uint32_t data = (uint32_t)(frames * 4), u32v;
    uint16_t u16v;
    if (!f)
        return;
#define W32(v) (u32v = (v), fwrite(&u32v, 4, 1, f))
#define W16(v) (u16v = (v), fwrite(&u16v, 2, 1, f))
    fwrite("RIFF", 1, 4, f);
    W32(36 + data);
    fwrite("WAVEfmt ", 1, 8, f);
    W32(16);
    W16(1);
    W16(2);
    W32(rate);
    W32(rate * 4);
    W16(4);
    W16(16);
    fwrite("data", 1, 4, f);
    W32(data);
    fwrite(pcm, 4, frames, f);
    fclose(f);
}

/* ---- --direct: the interpreter alone on a known sound (no game code) ---- */

int t2ultra_load_rom(const char *path);
void t2ultra_configure(const T2PortConfig *cfg);

#define D_IN 0x000   /* compressed bytes */
#define D_DEC 0x200  /* decoded (resampler input) */
#define D_RS 0x500   /* resampled */
#define D_L 0x800
#define D_R 0xB00
#define D_WL 0xE00
#define D_WR 0x1100
#define D_OUT 0x1400 /* interleaved */
#define FRAME 368    /* gAudioFrameSize */

static u32 *cmd(u32 *p, u32 w0, u32 w1) {
    p[0] = w0;
    p[1] = w1;
    return p + 2;
}
/* the 32-bit address an RSP command word holds: osVirtualToPhysical (t2ultra.c) gives a pointer
 * below 2 GB as itself and any other host address a region handle that T2Asp's ram() resolves
 * (the PIE build, the default since round 10, puts the test's heap above 4 GB: a plain cast
 * truncated it and --direct crashed in A_LOADADPCM) */
#define A32(p) ((u32)osVirtualToPhysical((void *)(p)))

/* Plays sound K of the sfx bank (instrument 0) at pitch 1.0 through a libaudio-shaped command
 * list per frame (LOADADPCM, LOADBUFF, ADPCM, RESAMPLE, SETVOL, ENVMIXER, INTERLEAVE, SAVEBUFF).
 * Writes the left channel as raw s16 (host order) to rawPath: at pitch 1.0 and full volume it is
 * the VADPCM decode delayed by 3 samples (the resampler's history), to compare with
 * tools/assets/audio.py's vadpcm_decode. */
static int run_direct(const char *rom, int k, const char *rawPath, const char *wavPath) {
    T2PortConfig cfg;
    const T2AudioCtl *ctl;
    ALBankFile *bf;
    ALInstrument *inst;
    ALSound *snd;
    ALWaveTable *w;
    ALADPCMBook *book;
    const u8 *data;
    static u32 list[2 * 64];
    static s16 out[FRAME * 2];
    s16 *adpcmState = t2_lowmem_alloc(32), *rsState = t2_lowmem_alloc(32), *envState = t2_lowmem_alloc(80);
    s16 *outBuf;
    u8 *bookBuf;
    int frames, f, nsamp, peak = 0;
    double sumsq = 0;
    FILE *raw, *wavf = NULL;
    int16_t *pcm;
    memset(&cfg, 0, sizeof cfg);
#ifdef T2_LOW_MEMORY_DEFAULT
    cfg.lowMemory = 1;
#endif
    t2ultra_configure(&cfg);
    if (t2ultra_load_rom(rom) != 0)
        return 1;
    adpcmState = t2_lowmem_alloc(32), rsState = t2_lowmem_alloc(32), envState = t2_lowmem_alloc(80);
    outBuf = t2_lowmem_alloc(FRAME * 4);
    ctl = T2Audio_LoadCtl();
    bf = ctl->sfxBank;
    inst = bf->bankArray[0]->instArray[0];
    if (k >= inst->soundCount) {
        fprintf(stderr, "direct: sound %d >= %d\n", k, inst->soundCount);
        return 1;
    }
    snd = inst->soundArray[k];
    w = snd->wavetable;
    book = w->waveInfo.adpcmWave.book;
    if (w->type != AL_ADPCM_WAVE || book == NULL || book->order != 2) {
        fprintf(stderr, "direct: sound %d is not order-2 ADPCM\n", k);
        return 1;
    }
    data = T2Audio_TblBytes(0x20F194u + (u32)(uptr)w->base, (u32)w->len);
    if (data == NULL) {
        /* T2Audio_TblBytes needs gAudioRomBase (set by CAudioWorld__Initialize); here: the ROM */
        size_t rs;
        data = t2_rom(&rs) + 0x20F194u + (u32)(uptr)w->base;
    }
    { /* the command words carry 32-bit addresses: a copy below 2 GB */
        u8 *c = t2_lowmem_alloc((size_t)w->len + 64);
        memcpy(c, data, (size_t)w->len);
        data = c;
    }
    /* the codebook as the converted bank holds it (host s16) */
    bookBuf = t2_lowmem_alloc((size_t)book->npredictors * 32);
    memcpy(bookBuf, book->book, (size_t)book->npredictors * 32);
    nsamp = (w->len / 9) * 16;
    frames = (nsamp + FRAME - 1) / FRAME;
    printf("direct: sfx sound %d: wave ROM %X, %d bytes, %d samples, %d predictors, %d frames\n", k,
           0x20F194u + (u32)(uptr)w->base, (int)w->len, nsamp, (int)book->npredictors, frames);
    raw = fopen(rawPath, "wb");
    pcm = calloc((size_t)frames * FRAME, 4);
    for (f = 0; f < frames; f++) {
        u32 *p = list;
        int first = f == 0;
        const u8 *src = data + (size_t)f * (FRAME / 16) * 9;
        u32 nbytes = (FRAME / 16) * 9;
        int i;
        p = cmd(p, 7u << 24, 0);                                                   /* A_SEGMENT */
        p = cmd(p, (11u << 24) | (u32)(book->npredictors * 32), A32(bookBuf));      /* A_LOADADPCM */
        p = cmd(p, (8u << 24) | D_IN, (0u << 16) | ((nbytes + 7) & ~7u));           /* A_SETBUFF */
        p = cmd(p, 4u << 24, A32(src));                                             /* A_LOADBUFF */
        p = cmd(p, (8u << 24) | D_IN, ((u32)D_DEC << 16) | (FRAME * 2));
        p = cmd(p, (1u << 24) | ((first ? 1u : 0u) << 16), A32(adpcmState));       /* A_ADPCM */
        p = cmd(p, (8u << 24) | (D_DEC + 32), ((u32)D_RS << 16) | (FRAME * 2)); /* past the previous frame */
        p = cmd(p, (5u << 24) | ((first ? 1u : 0u) << 16) | 0x8000, A32(rsState)); /* A_RESAMPLE 1.0 */
        p = cmd(p, (2u << 24) | D_L, FRAME * 2);                                    /* A_CLEARBUFF */
        p = cmd(p, (2u << 24) | D_R, FRAME * 2);
        p = cmd(p, (2u << 24) | D_WL, FRAME * 2);
        p = cmd(p, (2u << 24) | D_WR, FRAME * 2);
        p = cmd(p, (9u << 24) | (6u << 16) | 0x7FFF, 0);                            /* A_SETVOL L vol */
        p = cmd(p, (9u << 24) | (4u << 16) | 0x7FFF, 0);                            /* R vol */
        p = cmd(p, (9u << 24) | (2u << 16) | 0x7FFF, 0x00010000);                   /* L target/rate */
        p = cmd(p, (9u << 24) | (0u << 16) | 0x7FFF, 0x00010000);                   /* R target/rate */
        p = cmd(p, (9u << 24) | (8u << 16) | 0x7FFF, 0x0000);                       /* dry, wet */
        p = cmd(p, (8u << 24) | D_RS, ((u32)D_L << 16) | (FRAME * 2));
        p = cmd(p, (8u << 24) | (8u << 16) | D_R, ((u32)D_WL << 16) | D_WR);
        p = cmd(p, (3u << 24) | ((first ? 9u : 8u) << 16), A32(envState));         /* A_ENVMIXER */
        p = cmd(p, (8u << 24), ((u32)D_OUT << 16) | (FRAME * 2));
        p = cmd(p, 13u << 24, ((u32)D_L << 16) | D_R);                              /* A_INTERLEAVE */
        p = cmd(p, (8u << 24), ((u32)D_OUT << 16) | (FRAME * 4));
        p = cmd(p, 6u << 24, A32(outBuf));                                          /* A_SAVEBUFF */
        T2Asp_Run(list, (size_t)(p - list) / 2);
        memcpy(out, outBuf, sizeof out);
        memcpy(pcm + (size_t)f * FRAME * 2, out, sizeof out);
        for (i = 0; i < FRAME; i++) {
            int v = out[2 * i];
            fwrite(&out[2 * i], 2, 1, raw);
            if ((v < 0 ? -v : v) > peak)
                peak = v < 0 ? -v : v;
            sumsq += (double)v * v;
        }
    }
    fclose(raw);
    if (wavPath) {
        wav_write(wavPath, pcm, (size_t)frames * FRAME, 22050);
    }
    printf("direct: %d frames, left peak %d, RMS %.1f; raw left channel in %s\n", frames, peak,
           sqrt(sumsq / ((double)frames * FRAME)), rawPath);
    return peak > 200 ? 0 : 1;
}

/* ---- --adec ROMADDR SIZE out.raw: the host adec decoder (t2audio_adec.c) on one speech stream
 * of the ROM, to compare with tools/assets/audio.py (AdecDecoder) ---- */
int T2Audio_AdecDecode(const uint8_t *romBytes, size_t size, int16_t *out, size_t maxSamples, uint32_t *rate);
static int run_adec(const char *rom, unsigned addr, unsigned size, const char *rawPath) {
    T2PortConfig cfg;
    size_t rs;
    const uint8_t *r;
    int16_t *pcm;
    uint32_t rate;
    int n;
    FILE *fp;
    memset(&cfg, 0, sizeof cfg);
    t2ultra_configure(&cfg);
    if (t2ultra_load_rom(rom) != 0)
        return 1;
    r = t2_rom(&rs);
    if (addr + (size_t)size > rs)
        return 1;
    pcm = calloc(1 << 22, sizeof(int16_t));
    n = T2Audio_AdecDecode(r + addr, size, pcm, 1 << 22, &rate);
    fp = fopen(rawPath, "wb");
    if (fp == NULL || n < 0)
        return 1;
    fwrite(pcm, sizeof(int16_t), (size_t)n, fp);
    fclose(fp);
    printf("adec: ROM %X (%u bytes): %d samples at %u Hz -> %s\n", addr, size, n, rate, rawPath);
    free(pcm);
    return 0;
}

int main(int argc, char **argv) {
    T2PortConfig cfg;
    const char *wav = NULL;
    int16_t *all;
    size_t total = 0, cap;
    Stats st[3];
    int f, i, ok = 1;
    T2AudioStats as;
    T2AspStats ps;
    if (argc < 2) {
        fprintf(stderr, "usage: %s baserom.us.z64 [--wav out.wav] [--frames N] [--sfx K] [--song S]\n", argv[0]);
        return 2;
    }
    for (i = 2; i < argc; i++) {
        if (!strcmp(argv[i], "--adec") && i + 3 < argc)
            return run_adec(argv[1], (unsigned)strtoul(argv[i + 1], NULL, 0), (unsigned)strtoul(argv[i + 2], NULL, 0),
                            argv[i + 3]);
        if (!strcmp(argv[i], "--direct") && i + 2 < argc) {
            int k = atoi(argv[i + 1]);
            const char *rawp = argv[i + 2];
            const char *wv = (i + 4 < argc && !strcmp(argv[i + 3], "--wav")) ? argv[i + 4] : NULL;
            return run_direct(argv[1], k, rawp, wv);
        }
        if (!strcmp(argv[i], "--wav") && i + 1 < argc)
            wav = argv[++i];
        else if (!strcmp(argv[i], "--frames") && i + 1 < argc)
            sFrames = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--sfx") && i + 1 < argc)
            sSfx = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--song") && i + 1 < argc)
            sSong = atoi(argv[++i]);
    }
    memset(&cfg, 0, sizeof cfg);
    memset(st, 0, sizeof st);
    cfg.romPath = argv[1];
#ifdef T2_LOW_MEMORY_DEFAULT
    cfg.lowMemory = 1;
#endif
    cfg.audio = 1;
    cfg.mainEntry = test_main;
    if (T2Port_Boot(&cfg) != 0)
        return 1;
    cap = (size_t)sFrames * 3 * 400 + 4096;
    all = calloc(cap, 4);
    for (f = 0; f < sFrames * 3; f++) {
        int phase = f / sFrames; /* 0 silence, 1 sfx, 2 song */
        size_t n;
        if (phase != sPhase)
            sPhase = phase;
        T2Port_Frame();
        n = T2Audio_TakeSamples(all + 2 * total, (cap - total));
        stats_add(&st[phase], all + 2 * total, n);
        total += n;
    }
    T2Audio_GetStats(&as);
    T2Asp_GetStats(&ps);
    printf("t2audio_test: init %s; %llu synth frames, %llu stereo samples (%zu taken), %llu DMA calls\n",
           sInitOk ? "ok" : "FAILED", (unsigned long long)as.frames, (unsigned long long)as.samples, total,
           (unsigned long long)as.dmaCalls);
    printf("  ABI1: %llu lists, %llu commands, %llu unknown; per command:", (unsigned long long)ps.lists,
           (unsigned long long)ps.commands, (unsigned long long)ps.unknown);
    for (i = 0; i < 16; i++)
        printf(" %llu", (unsigned long long)ps.perCmd[i]);
    printf("\n");
    /* phase 0: nothing playing yet: must be (near) silent */
    printf("  silence: peak %d (expected 0)\n", st[0].peak);
    ok &= sInitOk && as.frames > 0 && st[0].peak < 64;
    ok &= stats_check("sfx", &st[1]);
    if (sSong >= 0)
        ok &= stats_check("song", &st[2]);
    if (wav) {
        wav_write(wav, all, total, T2Audio_Rate());
        printf("  wrote %s (%zu frames, %u Hz)\n", wav, total, (unsigned)T2Audio_Rate());
    }
    printf("t2audio_test: %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}
