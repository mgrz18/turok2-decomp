/* t2asp.c: the audio microcode (aspMain, libultra's ABI1) as a host interpreter
 * (docs/port/AUDIO.md §4).
 *
 * Written from the public ABI (PR/abi.h: command numbers, flags and the word layout of each
 * aXxx macro), libaudio's documented use of each command (the game's own builders in
 * src/us/code: alAudioFrame, _decodeChunk, alResamplePull, _pullSubFrame, alMainBusPull,
 * alAuxBusPull, alSavePull, the reverb) and the repository's VADPCM decoder
 * (tools/assets/audio.py vadpcm_decode). It is not bit-exact with the RSP: it reproduces what
 * each command means, with the RSP's fixed-point conventions where libaudio depends on them
 * (Q15 gains and volumes, 16.16 volume ramps applied every 8 samples as _getVol models them,
 * pitch as Q1.15 = 16.16 increment / 2).
 *
 * Byte order. DMEM is modelled as host-order s16 for sample buffers. Every buffer the commands
 * load or save is written by this interpreter or by host C (decoder state, reverb delay lines,
 * the ADPCM codebooks of the converted bank, the adec decoder's PCM), so it is host order too.
 * The one byte stream, VADPCM frames from the ROM, is read byte by byte.
 *
 * Addresses. The command words hold 32-bit addresses (osVirtualToPhysical, t2ultra.c): the
 * host address itself below 2 GB (low memory mode), a region handle above; ram() resolves both
 * through T2Port_PhysToVirtual. Segments (A_SEGMENT) are recorded; the game only
 * ever sets segment 0 to 0 (alAudioFrame), so addresses are used as they are. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "t2audio.h"

enum {
    A_SPNOOP = 0,
    A_ADPCM = 1,
    A_CLEARBUFF = 2,
    A_ENVMIXER = 3,
    A_LOADBUFF = 4,
    A_RESAMPLE = 5,
    A_SAVEBUFF = 6,
    A_SEGMENT = 7,
    A_SETBUFF = 8,
    A_SETVOL = 9,
    A_DMEMMOVE = 10,
    A_LOADADPCM = 11,
    A_MIXER = 12,
    A_INTERLEAVE = 13,
    A_POLEF = 14,
    A_SETLOOP = 15
};
#define A_INIT 0x01
#define A_LOOP 0x02
#define A_LEFT 0x02
#define A_VOL 0x04
#define A_AUX 0x08

#define DMEM_SIZE 0x10000

static union {
    uint8_t b[DMEM_SIZE + 64];
    int16_t h[(DMEM_SIZE + 64) / 2];
} sDmem;
static int16_t sBook[0x400]; /* A_LOADADPCM: ADPCM codebooks / pole filter coefficients */

/* A_SETBUFF registers */
static uint16_t sIn, sOut, sCount;
static uint16_t sDryRight, sWetLeft, sWetRight; /* A_SETBUFF | A_AUX */
/* A_SETVOL registers */
static int16_t sVol[2], sTarget[2], sDry, sWet;
static int32_t sRate[2];
static uint32_t sLoop;
static uint32_t sSeg[16];

static T2AspStats sStats;

void T2Asp_GetStats(T2AspStats *out) {
    *out = sStats;
}

static inline int16_t sat16(int32_t v) {
    return (int16_t)(v > 32767 ? 32767 : v < -32768 ? -32768 : v);
}
static inline int16_t *D16(uint32_t a) {
    return (int16_t *)(sDmem.b + (a & (DMEM_SIZE - 2)));
}
static inline uint8_t *D8(uint32_t a) {
    return sDmem.b + (a & (DMEM_SIZE - 1));
}
void *T2Port_PhysToVirtual(uint32_t h); /* t2ultra.c */
static inline void *ram(uint32_t a) {
    return T2Port_PhysToVirtual(a);
}

/* A_ADPCM: VADPCM, order 2 (the microcode's), 9-byte frames -> 16 samples, after the 16 samples of
 * the previous frame */
static void cmd_adpcm(uint32_t w0, uint32_t w1) {
    unsigned flags = (w0 >> 16) & 0xFF;
    int16_t st[16];
    const uint8_t *src = D8(sIn);
    int16_t *dst = D16(sOut);
    int frames = (sCount + 31) / 32, f, j, i, k;
    if (flags & A_INIT)
        memset(st, 0, sizeof st);
    else if (flags & A_LOOP)
        memcpy(st, ram(sLoop), sizeof st);
    else
        memcpy(st, ram(w1), sizeof st);
    /* the microcode writes the previous frame (the state) first and the new frames after it:
     * alAdpcmPull then skips 32 bytes, or lastsam samples into that frame (*outp += ...) */
    memcpy(dst, st, sizeof st);
    dst += 16;
    for (f = 0; f < frames; f++, src += 9) {
        int scale = 1 << (src[0] >> 4);
        int pred = src[0] & 0xF;
        const int16_t *c1 = &sBook[pred * 16], *c2 = &sBook[pred * 16 + 8];
        int32_t ix[16];
        for (i = 0; i < 8; i++) {
            int hi = src[1 + i] >> 4, lo = src[1 + i] & 0xF;
            ix[2 * i] = (hi >= 8 ? hi - 16 : hi) * scale;
            ix[2 * i + 1] = (lo >= 8 ? lo - 16 : lo) * scale;
        }
        for (j = 0; j < 2; j++) {
            /* the two samples before this group of 8 */
            int32_t p2 = st[j == 0 ? 14 : 6], p1 = st[j == 0 ? 15 : 7];
            int32_t out[8];
            for (i = 0; i < 8; i++) {
                /* row i of the predictor matrix: c1[i]*p2 + c2[i]*p1 + sum_k<i c2[i-1-k]*ix[k] + 2048*ix[i] */
                int64_t acc = (int64_t)c1[i] * p2 + (int64_t)c2[i] * p1;
                for (k = 0; k < i; k++)
                    acc += (int64_t)c2[i - 1 - k] * ix[j * 8 + k];
                out[i] = (int32_t)(acc >> 11) + ix[j * 8 + i];
            }
            for (i = 0; i < 8; i++)
                st[j * 8 + i] = sat16(out[i]);
        }
        memcpy(dst + 16 * f, st, sizeof st);
    }
    memcpy(ram(w1), st, sizeof st);
}

static void cmd_clearbuff(uint32_t w0, uint32_t w1) {
    uint32_t d = w0 & 0xFFFF, c = w1 & 0xFFFF;
    if (d + c > DMEM_SIZE)
        c = DMEM_SIZE - d;
    memset(D8(d), 0, c);
}

static void cmd_loadbuff(uint32_t w1) {
    uint32_t c = sCount;
    if (sIn + c > DMEM_SIZE)
        c = DMEM_SIZE - sIn;
    memcpy(D8(sIn), ram(w1), c);
}

static void cmd_savebuff(uint32_t w1) {
    uint32_t c = sCount;
    if (sOut + c > DMEM_SIZE)
        c = DMEM_SIZE - sOut;
    memcpy(ram(w1), D8(sOut), c);
}

static void cmd_setbuff(uint32_t w0, uint32_t w1) {
    unsigned flags = (w0 >> 16) & 0xFF;
    if (flags & A_AUX) {
        sDryRight = (uint16_t)(w0 & 0xFFFF);
        sWetLeft = (uint16_t)(w1 >> 16);
        sWetRight = (uint16_t)(w1 & 0xFFFF);
    } else {
        sIn = (uint16_t)(w0 & 0xFFFF);
        sOut = (uint16_t)(w1 >> 16);
        sCount = (uint16_t)(w1 & 0xFFFF);
    }
}

static void cmd_setvol(uint32_t w0, uint32_t w1) {
    unsigned flags = (w0 >> 16) & 0xFF;
    int16_t v = (int16_t)(w0 & 0xFFFF);
    if (flags & A_AUX) {
        sDry = v;
        sWet = (int16_t)(w1 & 0xFFFF);
    } else if (flags & A_VOL) {
        sVol[(flags & A_LEFT) ? 0 : 1] = v;
    } else {
        int c = (flags & A_LEFT) ? 0 : 1;
        sTarget[c] = v;
        sRate[c] = (int32_t)w1;
    }
}

/* A_ENVMIXER state in the 40 shorts of ENVMIX_STATE (the layout is this interpreter's own:
 * only the microcode reads it) */
typedef struct {
    int32_t vol[2]; /* 16.16 */
    int32_t rate[2];
    int16_t target[2];
    int16_t dry, wet;
} EnvState;

static void cmd_envmixer(uint32_t w0, uint32_t w1) {
    unsigned flags = (w0 >> 16) & 0xFF;
    EnvState s;
    int n = sCount / 2, i, c;
    const int16_t *in = D16(sIn);
    int16_t *dl = D16(sOut), *dr = D16(sDryRight), *wl = D16(sWetLeft), *wr = D16(sWetRight);
    if (flags & A_INIT) {
        for (c = 0; c < 2; c++) {
            s.vol[c] = (int32_t)sVol[c] << 16;
            s.rate[c] = sRate[c];
            s.target[c] = sTarget[c];
        }
        s.dry = sDry;
        s.wet = sWet;
    } else {
        memcpy(&s, ram(w1), sizeof s);
    }
    for (i = 0; i < n; i++) {
        int32_t x = in[i], l, r;
        if (i && (i & 7) == 0) {
            for (c = 0; c < 2; c++) {
                int64_t v = ((int64_t)s.vol[c] * (uint32_t)s.rate[c]) >> 16;
                int64_t t = (int64_t)s.target[c] << 16;
                if ((uint32_t)s.rate[c] >= 0x10000u ? v > t : v < t)
                    v = t;
                if (v > 0x7FFF0000)
                    v = 0x7FFF0000;
                s.vol[c] = (int32_t)v;
            }
        }
        l = (x * (s.vol[0] >> 16)) >> 15;
        r = (x * (s.vol[1] >> 16)) >> 15;
        dl[i] = sat16(dl[i] + ((l * s.dry) >> 15));
        dr[i] = sat16(dr[i] + ((r * s.dry) >> 15));
        if (flags & A_AUX) {
            wl[i] = sat16(wl[i] + ((l * s.wet) >> 15));
            wr[i] = sat16(wr[i] + ((r * s.wet) >> 15));
        }
    }
    /* the ramp step of the last (partial or full) group of 8 */
    if (n && (n & 7) == 0) {
        for (c = 0; c < 2; c++) {
            int64_t v = ((int64_t)s.vol[c] * (uint32_t)s.rate[c]) >> 16;
            int64_t t = (int64_t)s.target[c] << 16;
            if ((uint32_t)s.rate[c] >= 0x10000u ? v > t : v < t)
                v = t;
            if (v > 0x7FFF0000)
                v = 0x7FFF0000;
            s.vol[c] = (int32_t)v;
        }
    }
    memcpy(ram(w1), &s, sizeof s);
}

/* A_RESAMPLE: state = the last 4 input samples + the 16-bit fraction (own layout) */
static void cmd_resample(uint32_t w0, uint32_t w1) {
    unsigned flags = (w0 >> 16) & 0xFF;
    uint32_t inc = (w0 & 0xFFFF) << 1; /* Q1.15 pitch -> 16.16 step */
    int16_t st[16];
    static int16_t tmp[4 + 0x8000];
    int nout = sCount / 2, i;
    uint32_t pos, consumed, need;
    const int16_t *in = D16(sIn);
    int16_t *out = D16(sOut);
    if (flags & A_INIT)
        memset(st, 0, sizeof st);
    else
        memcpy(st, ram(w1), sizeof st);
    pos = (uint16_t)st[4];
    need = (uint32_t)(((uint64_t)pos + (uint64_t)inc * (uint32_t)nout) >> 16) + 4;
    if (need > 0x8000)
        need = 0x8000;
    memcpy(tmp, st, 4 * sizeof(int16_t));
    memcpy(tmp + 4, in, need * sizeof(int16_t));
    for (i = 0; i < nout; i++) {
        uint32_t idx = pos >> 16;
        int32_t f = (int32_t)(pos & 0xFFFF);
        int32_t s0, s1, s2, s3, a, b, cc;
        int64_t v;
        if (idx + 3 >= 4 + need)
            idx = 4 + need - 4;
        s0 = tmp[idx], s1 = tmp[idx + 1], s2 = tmp[idx + 2], s3 = tmp[idx + 3];
        /* Catmull-Rom between s1 and s2 */
        a = -s0 + 3 * s1 - 3 * s2 + s3;
        b = 2 * s0 - 5 * s1 + 4 * s2 - s3;
        cc = -s0 + s2;
        v = (int64_t)a * f >> 16;
        v = (v + b) * f >> 16;
        v = (v + cc) * f >> 16;
        out[i] = sat16((int32_t)((v >> 1) + s1));
        pos += inc;
    }
    consumed = pos >> 16;
    if (consumed + 4 > 4 + need)
        consumed = need;
    memcpy(st, tmp + consumed, 4 * sizeof(int16_t));
    st[4] = (int16_t)(pos & 0xFFFF);
    memcpy(ram(w1), st, sizeof st);
}

static void cmd_dmemmove(uint32_t w0, uint32_t w1) {
    uint32_t i = w0 & 0xFFFF, o = w1 >> 16, c = w1 & 0xFFFF;
    if (i + c > DMEM_SIZE || o + c > DMEM_SIZE)
        return;
    memmove(D8(o), D8(i), c);
}

static void cmd_loadadpcm(uint32_t w0, uint32_t w1) {
    uint32_t c = w0 & 0xFFFF;
    if (c > sizeof sBook)
        c = sizeof sBook;
    memcpy(sBook, ram(w1), c);
}

static void cmd_mixer(uint32_t w0, uint32_t w1) {
    int32_t gain = (int16_t)(w0 & 0xFFFF);
    const int16_t *in = D16(w1 >> 16);
    int16_t *out = D16(w1 & 0xFFFF);
    int n = sCount / 2, i;
    for (i = 0; i < n; i++)
        out[i] = sat16(out[i] + ((in[i] * gain) >> 15));
}

static void cmd_interleave(uint32_t w1) {
    static int16_t l[0x4000], r[0x4000];
    int n = sCount / 2, i;
    int16_t *out = D16(sOut);
    if (n > 0x4000)
        n = 0x4000;
    memcpy(l, D16(w1 >> 16), (size_t)n * 2);
    memcpy(r, D16(w1 & 0xFFFF), (size_t)n * 2);
    for (i = 0; i < n; i++) {
        out[2 * i] = l[i];
        out[2 * i + 1] = r[i];
    }
}

/* A_POLEF: one-pole low-pass y = (gain*x + c*y[-1]) >> 14, c = the 9th coefficient
 * (libaudio's _init_lpfilter: fccoef[8] = the time constant, gain = 16384 - constant) */
static void cmd_polef(uint32_t w0, uint32_t w1) {
    unsigned flags = (w0 >> 16) & 0xFF;
    int32_t gain = (int16_t)(w0 & 0xFFFF), c = sBook[8];
    int16_t st[4];
    const int16_t *in = D16(sIn);
    int16_t *out = D16(sOut);
    int n = sCount / 2, i;
    int32_t y;
    if (flags & A_INIT)
        memset(st, 0, sizeof st);
    else
        memcpy(st, ram(w1), sizeof st);
    y = st[3];
    for (i = 0; i < n; i++) {
        y = sat16((gain * in[i] + c * y) >> 14);
        out[i] = (int16_t)y;
    }
    st[3] = (int16_t)y;
    memcpy(ram(w1), st, sizeof st);
}

void T2Asp_Run(const uint32_t *cmds, size_t count) {
    size_t i;
    sStats.lists++;
    for (i = 0; i < count; i++) {
        uint32_t w0 = cmds[2 * i], w1 = cmds[2 * i + 1];
        unsigned op = w0 >> 24;
        sStats.commands++;
        if (op < 16)
            sStats.perCmd[op]++;
        switch (op) {
            case A_SPNOOP:
                break;
            case A_ADPCM:
                cmd_adpcm(w0, w1);
                break;
            case A_CLEARBUFF:
                cmd_clearbuff(w0, w1);
                break;
            case A_ENVMIXER:
                cmd_envmixer(w0, w1);
                break;
            case A_LOADBUFF:
                cmd_loadbuff(w1);
                break;
            case A_RESAMPLE:
                cmd_resample(w0, w1);
                break;
            case A_SAVEBUFF:
                cmd_savebuff(w1);
                break;
            case A_SEGMENT:
                sSeg[(w1 >> 24) & 0xF] = w1 & 0xFFFFFF;
                break;
            case A_SETBUFF:
                cmd_setbuff(w0, w1);
                break;
            case A_SETVOL:
                cmd_setvol(w0, w1);
                break;
            case A_DMEMMOVE:
                cmd_dmemmove(w0, w1);
                break;
            case A_LOADADPCM:
                cmd_loadadpcm(w0, w1);
                break;
            case A_MIXER:
                cmd_mixer(w0, w1);
                break;
            case A_INTERLEAVE:
                cmd_interleave(w1);
                break;
            case A_POLEF:
                cmd_polef(w0, w1);
                break;
            case A_SETLOOP:
                sLoop = w1;
                break;
            default:
                sStats.unknown++;
                break;
        }
    }
}
