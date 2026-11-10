/* t2audio_adec.c: the Acclaim "adec" speech streams on the host (docs/port/AUDIO.md §10).
 *
 * The speech player (SpeechPlayer_Update, t2audio_world.c) plays a song block of the sound data
 * file that is an adec stream through the sound player: a wave table of type 2 whose loader pull
 * is alAdecPull, which takes one decoded 256-sample frame at a time from adecNextFrame. The
 * game's decoder (adecStreamOpen, adecNextFrame, adecDecodeFrame, adecHuffDecode,
 * adecDequantize, adecImdctFrame, mdctFft: Rage Wars imports reading their state through
 * N64-offset views, and alAdecPull keeping the frame pointer in an s32) is replaced by this file,
 * a C version of tools/assets/audio.py's AdecDecoder (the Python reference):
 *
 *   header   u32 sampleRate, u32 frameCount, u32 bitstreamBytes; then the bit stream (MSB first)
 *   frame    16 band scales (6 bits each in the first frame, Huffman deltas after), 256
 *            coefficients (Huffman: escapes, literal values, phrases, end-of-frame), dequantized
 *            by the band's step, then an IMDCT (pre-twiddle, 128-point complex FFT, post-twiddle,
 *            windowed overlap-add) -> 256 s16 samples
 *   tables   the Huffman segment table gAdecHuffSegments (trees, phrases), gAdecBandEnds and
 *            gAdecStepSizes, read from the ROM image's code .rodata like the Python does
 *
 * In memory the stream is the sound data file's copy: T2Audio_SwapSoundData made the header
 * three host-order u32 and the bit stream host-order u16 words, so byte i of the stream is the
 * high (even i) or low (odd i) byte of word i / 2, whatever the host order. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "structs/libaudio.h"
#include "t2port.h"
#include "t2audio.h"

#define ADEC_CODE_VRAM_DELTA 0x7FFFF400u /* vram - rom of the code segment */
#define ADEC_SEGMENTS_VRAM 0x800B8C54u   /* gAdecHuffSegments */
#define ADEC_BANDENDS_VRAM 0x800B8A88u   /* gAdecBandEnds, u16[16] */
#define ADEC_STEPS_VRAM 0x800B8AA8u      /* gAdecStepSizes, f32[64] */
#define ADEC_MAX_SEGS 32
#define ADEC_SLOTS 6 /* adecNextFrame's ring of decoded frames (D_80125EF0, 6 x 0x200) */

typedef struct {
    s32 kind, start, len;
    u32 width;   /* literal width (value 0x900) */
    u32 phrases; /* ROM offset of the phrase table, 0 = none */
    u32 tree;    /* ROM offset of the tree */
} AdecSeg;

static struct {
    int ready;
    const u8 *rom;
    size_t romSize;
    AdecSeg segs[ADEC_MAX_SEGS];
    int nSegs, coefIdx[ADEC_MAX_SEGS], nCoef;
    u16 bandEnds[16];
    float steps[64];
    double pc[128], ps[128], qc[128], qs[128];
    double cosT[64], sinT[64];
    int bitrev[128];
} sT;

static struct {
    const u8 *data; /* the stream (header first) */
    u32 rate, frames, left, bit;
    int first;
    s16 band[16];
    double overlap[128];
    s16 slots[ADEC_SLOTS][256];
    int slot;
    int stop;
} sS;

static u32 rd32be(u32 a) {
    const u8 *p = sT.rom + a;
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
}
static u16 rd16be(u32 a) {
    const u8 *p = sT.rom + a;
    return (u16)((p[0] << 8) | p[1]);
}
static u32 rom_of(u32 vram) {
    return vram - ADEC_CODE_VRAM_DELTA;
}

static int adec_tables(void) {
    u32 a, covered = 0;
    int i;
    if (sT.ready)
        return sT.ready > 0;
    sT.ready = -1;
    sT.rom = t2_rom(&sT.romSize);
    if (sT.rom == NULL || sT.romSize < 0x100000) {
        t2_log_once("t2audio: adec: no ROM image for the decoder tables");
        return 0;
    }
    for (i = 0; i < 16; i++)
        sT.bandEnds[i] = rd16be(rom_of(ADEC_BANDENDS_VRAM) + 2 * i);
    for (i = 0; i < 64; i++) {
        u32 w = rd32be(rom_of(ADEC_STEPS_VRAM) + 4 * i);
        memcpy(&sT.steps[i], &w, 4);
    }
    if (sT.bandEnds[15] != 0x100 || sT.steps[63] != 1.0f) {
        t2_log_once("t2audio: adec: decoder tables not found");
        return 0;
    }
    for (a = rom_of(ADEC_SEGMENTS_VRAM); sT.nSegs < ADEC_MAX_SEGS; a += 0x14) {
        AdecSeg *s = &sT.segs[sT.nSegs++];
        u32 t = rom_of(rd32be(a + 16)), ph;
        s->kind = (s32)rd32be(a);
        s->start = (s32)rd32be(a + 4);
        s->len = (s32)rd32be(a + 8);
        s->width = rd16be(t + 0xE);
        ph = rd32be(t + 0x1C);
        s->phrases = ph ? rom_of(ph) : 0;
        s->tree = rom_of(rd32be(t + 0x20));
        if (s->kind == 0) {
            sT.coefIdx[sT.nCoef++] = sT.nSegs - 1;
            covered = (u32)(s->start + s->len);
            if (covered >= 256)
                break;
        }
    }
    for (i = 0; i < 128; i++) {
        sT.pc[i] = cos((i * 4.0 + 1.0) * 0.0030679617);
        sT.ps[i] = sin((i * 4.0 + 1.0) * 0.0030679617);
        sT.qc[i] = cos(i * 0.012271847) + sin(i * 0.012271847);
        sT.qs[i] = sin(i * 0.012271847) - cos(i * 0.012271847);
    }
    for (i = 0; i < 64; i++) {
        sT.cosT[i] = cos(i * 2 * 3.14159265358979323846 / 128);
        sT.sinT[i] = -sin(i * 2 * 3.14159265358979323846 / 128);
    }
    {
        /* mdctFft's bit-reversal order (audio.py _fft_tables) */
        int j = 1, k, m;
        for (i = 0; i < 128; i++)
            sT.bitrev[i] = i;
        for (k = 1; k < 256 + 1; k += 2) {
            if (k < j) {
                int t = sT.bitrev[(j - 1) / 2];
                sT.bitrev[(j - 1) / 2] = sT.bitrev[(k - 1) / 2];
                sT.bitrev[(k - 1) / 2] = t;
            }
            m = 128;
            while (m >= 2 && j > m) {
                j -= m;
                m >>= 1;
            }
            j += m;
        }
    }
    sT.ready = 1;
    return 1;
}

/* ---- the bit stream ---- */

static u32 adec_byte(u32 i) {
    const u16 *w = (const u16 *)(sS.data + 12);
    u16 v = w[i >> 1];
    return (i & 1) ? (v & 0xFF) : (v >> 8);
}
static u32 adec_bits(int n) {
    u32 v = 0;
    while (n-- > 0) {
        u32 byteIdx = sS.bit >> 3;
        u32 b = byteIdx < sS.left ? adec_byte(byteIdx) : 0;
        v = (v << 1) | ((b >> (7 - (sS.bit & 7))) & 1);
        sS.bit++;
    }
    return v;
}

/* Huffman values into out[0..n): the segments idx[0..nseg) in order (audio.py _huff) */
static void adec_huff(const int *idx, int nseg, int n, s32 *out) {
    int len = 0, b = 0;
    const AdecSeg *seg = &sT.segs[idx[0]];
    u32 node = 0;
    while (len < n) {
        u32 bit = adec_bits(1);
        u32 child = rd16be(seg->tree + 4 * node + (bit ? 2 : 0));
        u32 value, m;
        if (rd16be(seg->tree + 4 * child) != 0x800) {
            node = child;
            continue;
        }
        value = rd16be(seg->tree + 4 * child + 2);
        node = 0;
        m = value & 0xF00;
        if (m == 0 || m == 0xF00) {
            out[len++] = (value & 0x8000) ? (s32)value - 0x10000 : (s32)value;
        } else if (value == 0x900) {
            u32 v = adec_bits((int)seg->width);
            out[len++] = (v & 0x8000) ? (s32)v - 0x10000 : (s32)v;
        } else if (m == 0xA00) {
            u32 pa = seg->phrases + (value & 0xFF) * 0x1B8u;
            s32 cnt = seg->phrases ? (s32)rd32be(pa + 0x1A0) : 0, j;
            for (j = 0; j < cnt && len < n; j++)
                out[len++] = (s16)rd16be(pa + 0xA0 + 4 * j + 2);
        } else if (m == 0xB00) {
            while (len < n)
                out[len++] = 0;
            return;
        }
        if (len >= seg->start + seg->len && b + 1 < nseg) {
            b++;
            seg = &sT.segs[idx[b]];
        }
    }
}

static void adec_fft(double *d, double sign) {
    int i, n = 128, dist = 2, half = 1, stride = 64, nn = 256;
    for (i = 0; i < n; i++) {
        int j = sT.bitrev[i];
        if (i < j) {
            double t = d[2 * i];
            d[2 * i] = d[2 * j];
            d[2 * j] = t;
            t = d[2 * i + 1];
            d[2 * i + 1] = d[2 * j + 1];
            d[2 * j + 1] = t;
        }
    }
    while (stride > 0) {
        int w = 0, k, idx;
        for (k = 0; k < half; k++) {
            double wc = sT.cosT[w], ws = sign * sT.sinT[w];
            for (idx = k * 2; idx < nn; idx += dist * 2) {
                double xr = d[idx + dist], xi = d[idx + dist + 1];
                double tr = wc * xr - ws * xi, ti = wc * xi + ws * xr;
                d[idx + dist] = d[idx] - tr;
                d[idx + dist + 1] = d[idx + 1] - ti;
                d[idx] += tr;
                d[idx + 1] += ti;
            }
            w += stride;
        }
        stride >>= 1;
        half <<= 1;
        dist <<= 1;
    }
}

static s16 adec_clamp(double x) {
    double y = x >= 0 ? x + 0.5 : x - 0.5;
    if (y > 32767.0)
        y = 32767.0;
    if (y < -32768.0)
        y = -32768.0;
    return (s16)(int)y;
}

static void adec_frame(s16 *out) {
    static const int allSegs[ADEC_MAX_SEGS] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                                                16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
    s32 coef[256 + 16], delta[16 + 16];
    double deq[256], buf[256];
    int b, i, k, start = 0;
    if (sS.first) {
        for (b = 0; b < 16; b++)
            sS.band[b] = (s16)adec_bits(6);
        sS.first = 0;
    } else {
        adec_huff(allSegs, sT.nSegs, 16, delta);
        for (b = 0; b < 16; b++)
            sS.band[b] = (s16)(u16)(sS.band[b] + delta[b]);
    }
    adec_huff(sT.coefIdx, sT.nCoef, 256, coef);
    for (b = 0; b < 16; b++) {
        int s = sS.band[b] < 0 ? 0 : sS.band[b] > 63 ? 63 : sS.band[b];
        double step = sT.steps[s];
        for (i = start; i < sT.bandEnds[b]; i++)
            deq[i] = coef[i] / step * 3.0517578e-5;
        start = sT.bandEnds[b];
    }
    for (k = 0; k < 128; k++) {
        double re = deq[2 * k], im = deq[255 - 2 * k];
        buf[2 * k] = re * sT.pc[k] - im * sT.ps[k];
        buf[2 * k + 1] = re * sT.ps[k] + im * sT.pc[k];
    }
    adec_fft(buf, -1.0);
    for (k = 0; k < 128; k++) {
        double re = buf[2 * k], im = buf[2 * k + 1];
        buf[2 * k] = re * sT.qc[k] - im * sT.qs[k];
        buf[2 * k + 1] = re * sT.qs[k] + im * sT.qc[k];
    }
    for (k = 0; k < 128; k++) {
        double ov = sS.overlap[k], im = buf[2 * k + 1];
        double t1 = ov * sT.pc[k] + im * sT.ps[k];
        double t2 = ov * sT.ps[k] - im * sT.pc[k];
        sS.overlap[k] = buf[2 * k];
        out[2 * k] = adec_clamp(23170.475 * t1);
        out[255 - 2 * k] = adec_clamp(23170.475 * t2);
    }
}

/* ---- the game's interface ---- */

/* func_002A1170: opens a stream; *rate = its sample rate; returns its length in microseconds
 * at `divisor` Hz output (count * 256 * 1e6 / divisor, the speech sound's decay time) */
u32 adecStreamOpen(void *source, u32 divisor, s32 *rate) {
    u32 h[3];
    memcpy(h, source, sizeof h);
    memset(&sS, 0, sizeof sS);
    sS.data = source;
    sS.rate = h[0];
    sS.frames = h[1];
    sS.left = h[2];
    sS.first = 1;
    *rate = (s32)h[0];
    if (!adec_tables())
        sS.frames = 0;
    return (u32)(((u64)h[1] * 0xF424000u) / (divisor ? divisor : 1));
}

/* func_002A122C: the next decoded frame (256 host-order s16), NULL at the end */
s16 *adecNextFrame(void) {
    s16 *out;
    if (sS.frames == 0) {
        sS.stop = 1;
        return NULL;
    }
    sS.slot = (sS.slot + 1) % ADEC_SLOTS;
    out = sS.slots[sS.slot];
    adec_frame(out);
    sS.frames--;
    return out;
}

/* func_002A12CC / func_002A12DC */
s32 adecGetStopFlag(void) {
    return sS.stop;
}
void adecClearStopFlag(void) {
    sS.stop = 0;
}

/* func_0029F958: alAdecPull, the loader filter's pull for a type-2 (adec) wave table: whole
 * decoded frames loaded into DMEM at *outp (count = samples), aligned like the game does it */
u32 osVirtualToPhysical(void *p);
#define T2_ACMD(ptr, a, b)                  \
    do {                                    \
        Acmd *_a = (ptr);                   \
        _a->words.w0 = (unsigned int)(a);   \
        _a->words.w1 = (unsigned int)(b);   \
    } while (0)
static s16 *sAdecCur;
Acmd *alAdecPull(void *filter, s16 *outp, s32 count, s32 sampleOffset, Acmd *p) {
    ALLoadFilter *f = (ALLoadFilter *)filter;
    Acmd *ptr = p;
    s32 op = *outp;
    (void)sampleOffset;
    if (f->table->base == NULL) {
        T2_ACMD(ptr++, (2u << 24) | ((u32)op & 0xFFFFFF), (u32)count << 1); /* A_CLEARBUFF */
        return ptr;
    }
    while (count != 0) {
        s32 nSam = count, nbytes, dramAlign, dmemAlign;
        if (f->sample == 0) {
            sAdecCur = adecNextFrame();
            if (sAdecCur == NULL) {
                T2_ACMD(ptr++, (2u << 24) | ((u32)op & 0xFFFFFF), (u32)count << 1);
                return ptr;
            }
            f->sample = 0x100;
        }
        if (f->sample < count)
            nSam = f->sample;
        nbytes = nSam << 1;
        dramAlign = (s32)((uptr)sAdecCur & 7);
        nbytes += dramAlign;
        dmemAlign = (op & 7) ? 8 - (op & 7) : 0;
        T2_ACMD(ptr++, (8u << 24) | ((u32)(op + dmemAlign) & 0xFFFF), (u32)(nbytes + 8 - (nbytes & 7)) & 0xFFFF);
        T2_ACMD(ptr++, 4u << 24, osVirtualToPhysical((u8 *)sAdecCur - dramAlign)); /* A_LOADBUFF */
        if (dramAlign || dmemAlign)
            T2_ACMD(ptr++, (10u << 24) | ((u32)(op + dramAlign + dmemAlign) & 0xFFFFFF),
                    ((u32)op << 16) | ((u32)(nSam << 1) & 0xFFFF)); /* A_DMEMMOVE */
        count -= nSam;
        op += nSam << 1;
        sAdecCur += nSam;
        f->sample -= nSam;
    }
    return ptr;
}

/* t2audio_test --adec: decodes a stream given in ROM order (big-endian header and bytes) */
int T2Audio_AdecDecode(const u8 *romBytes, size_t size, s16 *out, size_t maxSamples, u32 *rate) {
    u8 *copy;
    size_t i, n = 0;
    s32 r;
    s16 *fr;
    if (size < 12)
        return -1;
    copy = malloc(size + 2);
    for (i = 0; i < 3; i++) {
        u32 v = ((u32)romBytes[4 * i] << 24) | ((u32)romBytes[4 * i + 1] << 16) | ((u32)romBytes[4 * i + 2] << 8) |
                romBytes[4 * i + 3];
        memcpy(copy + 4 * i, &v, 4);
    }
    for (i = 12; i + 1 < size; i += 2) {
        u16 v = (u16)((romBytes[i] << 8) | romBytes[i + 1]);
        memcpy(copy + i, &v, 2);
    }
    adecStreamOpen(copy, 22050, &r);
    *rate = (u32)r;
    while ((fr = adecNextFrame()) != NULL && n + 256 <= maxSamples) {
        memcpy(out + n, fr, 256 * sizeof(s16));
        n += 256;
    }
    free(copy);
    return (int)n;
}
