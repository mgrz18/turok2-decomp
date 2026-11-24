/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/adecReadBits.c, adecReadBits, with Turok 2's layouts. */

#include "common.h"
#include "globals/adec.h"
#include "structs/adec.h"

/* Acclaim compressed-audio stream decoder state (one instance) */

/* Bit reader: fetches `count` fields of `nbits` bits (MSB first) from the stream of 16-bit words at st->rd into dst[],
   sign-extending each field to 16 bits when `sign` is set. Always returns 1. */
s32 adecReadBits(u16 *dst, u16 nbits, u16 count, s16 sign, AdecStream *st)
{
    s16 i;
    u16 extra;     /* bits needed from the next word */
    u16 maskLeft;
    u16 curWord;
    int fldA;
    int fldB;
    if (sAdecBitsLeft == 0) {
        *(volatile u16 *)&sAdecBitWord = *(st->rd++);
        sAdecBitsLeft = 16;
    }
    for (i = 0; i < count; i++) {
        if (sAdecBitsLeft < nbits) {
            /* the field straddles two words: the low sAdecBitsLeft bits of this one, then the top `extra` bits of the next */
            extra = nbits - sAdecBitsLeft;
            curWord = sAdecBitWord;
            maskLeft = gAdecBitMasks[sAdecBitsLeft];
            dst[i] = (curWord & maskLeft) << extra;
            sAdecBitWord = *(st->rd++);
            sAdecBitWord += 0;
            fldA = (sAdecBitWord >> (16 - extra)) & gAdecBitMasks[extra];
            dst[i] |= fldA;
            if (sign) {
                if (((s16)dst[i]) & gAdecSignBits[nbits]) {
                    dst[i] |= ~gAdecBitMasks[nbits];
                }
            }
            sAdecBitsLeft = 16 - extra;
        } else {
            fldB = (sAdecBitWord >> (sAdecBitsLeft - nbits)) & gAdecBitMasks[nbits];
            dst[i] = fldB;
            if (sign) {
                if (((s16)dst[i]) & gAdecSignBits[nbits]) {
                    dst[i] |= ~gAdecBitMasks[nbits];
                }
            }
            sAdecBitsLeft -= nbits;
        }
    }
    return 1;
}

/* Re-matched under -fforce-addr (round 26), the flags of the other adecHuffDecode files: the
 * phrase address is an integer sum (dictOffset first), no longer the (phrases + 1) - 1 form. */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_12.c, func_802BEF84_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#ifdef TARGET_PORT
#define readBits adecReadBits
#else
/* The ROM caller takes the result as a u16 (andi 0xFFFF): called through a type of its own. */
#define readBits ((u16 (*)(u16 *, u16, u16, s16, AdecStream *))adecReadBits)
#endif

struct DecodeBlock;
s16 adecHuffDecode(DecodeBlock *arg0, u16 *arg1, s16 arg2, u16 *arg3, AdecStream *arg4);

s16 adecHuffDecode(DecodeBlock *arg0, u16 *arg1, s16 arg2,
                    u16 *arg3, AdecStream *arg4) {
    u16 node;
    s16 block, output;
    DecodeTable *table;
    DecodeNode *tree;
    u16 value;
    s32 mask, high;
    u16 topbit;
    s16 j;
    s32 dictOffset;
    u16 next;
    u16 *bits, *available;
    DecodePhrase *phrase;
    u16 *out;
    node = 0;
    block = 0;
    *arg3 = 0;
    table = arg0->table;
    tree = table->tree;
    output = 0;
    if (arg2 > 0) {
        available = &sAdecHuffBitsAvail;
        bits = &sAdecHuffBits;
        do {
            if (*available != 0) {
                (*available)--;
                topbit = *bits & 0x8000;
                *bits <<= 1;
                if (topbit) {
                    mask = tree[(s16)node].second;
                } else {
                    mask = tree[(s16)node].first;
                }
                if (tree[(s16)mask].first == 0x800) {
                    value = tree[(s16)mask].second;
                    mask = value & 0xF00;
                    node = 0;
                    if (mask == 0 || mask == 0xF00) {
                        arg1[(s16)output] = value;
                        output++;
                    } else if ((s16)value == 0x900) {
                        value = *bits;
                        if (readBits(bits, table->width, 1, 0, arg4) == 0) {
                            return -1;
                        }
                        mask = (s16)output;
                        output++;
                        high = *bits;
                        next = sAdecHuffBitsAvail;
                        *bits = high << (16 - next);
                        arg1[(s16)mask] = value | (high >> next);
                    } else if (mask == 0xA00) {
                        j = 0;
                        dictOffset = (value & 0xFF) * sizeof(DecodePhrase);
                        out = (arg1 + ((s16)output + 1)) - 1;
                        phrase = (DecodePhrase *)(dictOffset + (sptr)table->phrases);
                        while ((s16)j < phrase->count) {
                            out[(s16)j] = phrase->samples[(s16)j].value;
                            j++;
                        }
                        output += (s32)((u32)j << 16) >> 16;
                    } else if (mask == 0xB00) {
                        while ((s16)output < arg2) {
                            arg1[(s16)output++] = 0;
                        }
                        while (arg2 >= arg0[(s16)block].start +
                                       arg0[(s16)block].length) {
                            block++;
                            (*arg3)++;
                        }
                        return arg2;
                    }
                    if ((s16)output >= arg0[(s16)block].start +
                                       arg0[(s16)block].length) {
                        block++;
                        (*arg3)++;
                        table = arg0[(s16)block].table;
                        tree = table->tree;
                    }
                } else {
                    node = mask;
                }
            } else {
                if (readBits(&sAdecHuffBits, 16, 1, 0, arg4) == 0) {
                    return -1;
                }
                *available += 16;
            }
        } while ((s16)output < arg2);
    }
    return (s16)output;
}

/* Decodes one frame into st: the 16 scale indices st->band[] (the very first frame raw 6 bit fields, afterwards Huffman coded
   differences against the previous frame) and the 256 coefficients st->coef[]. Returns 1, or -1 on a stream error. */
s32 adecDecodeFrame(AdecStream *st) {
    s16 cnt;
    s16 nv;
    s16 i;
    s32 rv;
    AdecHuffSeg *seg = gAdecHuffSegments;
    if (sAdecHeaderDone == 0) {
        sAdecHeaderDone = 1;
        if ((s16)adecReadBits((u16 *)st->band, 6, 16, 0, st) <= 0) {
            return -1;
        }
        seg->pos += 0x60;
        for (i = 0; i < 16; i++) {
            sAdecPrevScale[i] = st->band[i];
        }
        while (seg->kind != 0) {
            seg++;
        }
    } else {
        rv = adecHuffDecode((struct DecodeBlock *)seg, (u16 *)st->band, 16, (u16 *)&cnt, st);
        if (rv != 16) {
            goto fail;
        }
        seg = &seg[nv = cnt];
        for (i = 0; i < 16; i++) {
            nv = sAdecPrevScale[i];
            st->band[i] += nv;
            sAdecPrevScale[i] = st->band[i];
        }
    }
    rv = adecHuffDecode((struct DecodeBlock *)seg, (u16 *)st->coef, 256, (u16 *)&cnt, st);
    if (rv != 256) {
    fail:
        return -1;
    }
    return 1;
}

extern float func_002974C0(float x);
extern float func_002982F0(float x);
void mdctInitTwiddles(void);

void mdctInitTwiddles(void) {
    s16 i;
    f32 angle;
    f32 s;
    f32 c;
    f32 step;
    f32 offset;
    f32 scale;
    f32 *sines;
    f32 *cosines;
    s32 *zeros;
    f32 *sums;
    f32 *differences;
    step = 4.0f;
    offset = (1.0f);
    scale = 0.0030679617f;
    i = 0;
    sines = sMdctPreCos;
    cosines = sMdctPreSin;
    zeros = sAdecOverlap;
    for (; i < 128; i++) {
        angle = (i * step + offset) * scale;
        sines[i] = func_002974C0(angle);
        cosines[i] = func_002982F0(angle);
        zeros[i] = 0;
    }
    i = 0;
    scale = (0.012271847f);
    sums = sMdctPostCosPlusSin;
    differences = sMdctPostSinMinusCos;
    for (; i < 128; i++) {
        angle = i * scale;
        s = func_002974C0(angle);
        c = func_002982F0(angle);
        sums[i] = s + c;
        differences[i] = c - s;
    }
}
FftTables *mdctFftInit(s32 n);

FftTables *mdctFftInit(s32 n) {
    FftTables *tables;
    s32 half;
    s32 i;
    s32 k;
    s32 j;
    s32 m;
    s32 swap;
    s32 limit;
    s32 *order;
    f32 angle;
    f32 scale;
    f32 count;
    half = n >> 1;
    tables = &gMdctFftTables;
    i = 0;
    if (i < half) {
        scale = 6.2831855f;
        count = n;
        do {
            angle = i * scale / count;
            tables->cos[i] = func_002974C0(angle);
            tables->sin[i] = -func_002982F0(angle);
            i++;
        } while (i < half);
    }
    order = tables->bitrev;
    limit = n * 2;
    for (k = 0; k < n; k++) {
        order[k] = k;
    }
    j = 1;
    for (k = 1; k <= limit; k += 2) {
        if (k < j) {
            swap = order[(j - 1) / 2];
            order[(j - 1) / 2] = order[(k - 1) / 2];
            order[(k - 1) / 2] = swap;
        }
        m = n;
        while (m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    tables->n = n;
    return tables;
}
s32 adecDequantize(f32 *out, s16 *in);

s32 adecDequantize(f32 *out, s16 *in) {
    s16 *scale;
    s16 band;
    s16 i;
    s16 start;
    s16 *values;
    u16 *ends;
    f32 *steps;
    f32 gain;
    start = 0;
    values = in + 0x50;
    band = 0;
    ends = gAdecBandEnds;
    steps = gAdecStepSizes;
    gain = 3.0517578e-5f;
    for (; band < 16; band++) {
        for (i = start; i < ends[band]; i++) {
            out[i] = (f32)values[i] / steps[*(scale = &in[band])] * gain;
        }
        start = ends[band];
    }
    for (band = gAdecBandEnds[15]; band < 0x100; band++) {
        out[band] = 0.0f;
    }
    return 1;
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/adecReadBits.c, mdctFft.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void mdctFft(f32 *d, s32 n, s32 sign, f32 *cos, f32 *sin, s32 *bitrev);

void mdctFft(f32 *d, s32 n, s32 sign, f32 *cos, f32 *sin, s32 *bitrev) {
    s32 i, k, half, dist, stride, idx, w, nn;
    f32 wc, ws, xr, xi, tr, ti;
    for (i = 0; i < n; i++) {
        if (i < bitrev[i]) {
            tr = d[i * 2];
            d[i * 2] = d[bitrev[i] * 2];
            d[bitrev[i] * 2] = tr;
            tr = d[i * 2 + 1];
            d[i * 2 + 1] = d[bitrev[i] * 2 + 1];
            d[bitrev[i] * 2 + 1] = tr;
        }
    }
    dist = 2;
    half = 1;
    stride = n >> half;
    nn = n << half;
    while (stride > 0) {
        w = 0;
        for (k = 0; k < half; k++) {
            wc = cos[w];
            ws = (f32)sign * sin[w];
            for (idx = k * 2; idx < nn; idx += dist * 2) {
                xr = d[idx + dist];
                xi = d[idx + dist + 1];
                tr = wc * xr - ws * xi;
                ti = wc * xi + ws * xr;
                d[idx + dist] = d[idx] - tr;
                d[idx + dist + 1] = d[idx + 1] - ti;
                d[idx] = d[idx] + tr;
                d[idx + 1] = d[idx + 1] + ti;
            }
            w += stride;
        }
        stride >>= 1;
        half <<= 1;
        dist <<= 1;
    }
}

/* Acclaim compressed-audio stream decoder state (one instance) */

typedef struct {
    f32 re;
    f32 im;
} AdecCpx;

/* out[k] = (in[2k], in[255 - 2k]) for k < 128: folds the 256 spectrum values into 128 complex points. */
static __inline__ void mdctPackSpectrum_2A0B6C(AdecCpx *out, f32 *in) {
    s16 k;
    for (k = 0; k < 128; k++) {
        out[k].re = in[k * 2];
        out[k].im = in[255 - k * 2];
    }
}

/* d[k] *= (c[k] + i s[k]) for the 128 complex points. */
static __inline__ void mdctTwiddleMul_2A0B6C(AdecCpx *d, f32 *c, f32 *s) {
    s16 k;
    for (k = 0; k < 128; k++) {
        f32 re = d[k].re;
        f32 cc = c[k];
        f32 im = d[k].im;
        f32 ss = s[k];
        d[k].re = re * cc - im * ss;
        d[k].im = re * ss + im * cc;
    }
}

/* FFT with the tables of an FftTables. */
static __inline__ void mdctFftCtx_2A0B6C(f32 *data, FftTables *ctx, s32 sign) {
    mdctFft(data, ctx->n, sign, ctx->cos, ctx->sin, ctx->bitrev);
}

/* Rounds to nearest (half away from zero) and clamps to the s16 range. */
static __inline__ s16 adecRoundClampS16_2A0B6C(f32 x) {
    f32 y;
    if (x >= 0.0f) {
        y = x + 0.5f;
        if (y > 32767.0f) {
            y = 32767.0f;
        }
    } else {
        y = x - 0.5f;
        if (y < -32768.0f) {
            y = -32768.0f;
        }
    }
    return (s16)y;
}

/* Inverse MDCT of the frame in st -> 256 s16 samples in out[]. The twiddle tables and the FFT context are built on first use. */
void adecImdctFrame(s16 *out, AdecStream *st) {
    f32 buf[256];
    f32 deq[256];
    s16 k;
    f32 scale;
    if (sAdecImdctReady == 0) {
        mdctInitTwiddles();
        sMdctFft = mdctFftInit(0x80);
        sAdecImdctReady = 1;
    }
    scale = 23170.475f;
    adecDequantize(deq, st->band);
    mdctPackSpectrum_2A0B6C((AdecCpx *)buf, deq);
    mdctTwiddleMul_2A0B6C((AdecCpx *)buf, sMdctPreCos, sMdctPreSin);
    mdctFftCtx_2A0B6C(buf, sMdctFft, -1);
    mdctTwiddleMul_2A0B6C((AdecCpx *)buf, sMdctPostCosPlusSin, sMdctPostSinMinusCos);
    for (k = 0; k < 128; k++) {
        f32 ov = sAdecOverlap[k];
        f32 cc = sMdctPreCos[k];
        f32 im = buf[k * 2 + 1];
        f32 ss = sMdctPreSin[k];
        f32 t1 = ov * cc + im * ss;
        f32 t2 = ov * ss - im * cc;
        sAdecOverlap[k] = buf[k * 2];
        out[k * 2] = adecRoundClampS16_2A0B6C(scale * t1);
        out[255 - k * 2] = adecRoundClampS16_2A0B6C(scale * t2);
    }
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS adecDecodeFrame received adecDecodeFrameOk's own argument
 * registers (pass-through); the host passes them. */
u32 adecDecodeFrameOk(AdecStream *st) {
#ifdef TARGET_PORT
    return (u32) ~adecDecodeFrame(st) >> 0x1F;
#else
    /* The ROM caller takes the result as an s16: called through a type of its own. */
    return (u32) ~((s16 (*)(AdecStream *))adecDecodeFrame)(st) >> 0x1F;
#endif
}
void mdctBuildBitrevTable(s32 *arg0, s32 arg1);

void mdctBuildBitrevTable(s32 *arg0, s32 arg1) {
    s32 *ptr;
    s32 i;
    s32 j;
    s32 k;
    s32 tmp;
    s32 n2;
    i = 0;
    n2 = arg1 * 2;
    if (arg1 > 0) {
        ptr = arg0;
        do {
            *ptr = i;
            i++;
            ptr++;
        } while (i < arg1);
    }
    j = 1;
    i = 1;
    if (n2 > 0) {
        do {
            if (j > i) {
                tmp = arg0[(j - 1) / 2];
                arg0[(j - 1) / 2] = arg0[(i - 1) / 2];
                arg0[(i - 1) / 2] = tmp;
            }
            k = arg1;
            while (k >= 2 && j > k) {
                j -= k;
                k >>= 1;
            }
            i += 2;
            j += k;
        } while (n2 >= i);
    }
}
void mdctPackSpectrum(float *arg0, float *arg1);

void mdctPackSpectrum(float *arg0, float *arg1) {
    short i = 0;
    do {
        arg0[i * 2] = arg1[i * 2];
        arg0[i * 2 + 1] = arg1[(255 - i * 2)];
        i++;
    } while (i < 128);
}
void mdctTwiddleMul(float *arg0, float *arg1, float *arg2);

void mdctTwiddleMul(float *arg0, float *arg1, float *arg2) {
    short i = 0;
    do {
        float re = arg0[i * 2];
        float im = arg0[i * 2 + 1];
        float a = arg1[i];
        float b = arg2[i];
        arg0[i * 2] = re * a - im * b;
        arg0[i * 2 + 1] = re * b + im * a;
        i++;
    } while (i < 0x80);
}

/* The FFT tables mdctFft needs. */
typedef struct {
    /* 0x0 */ s32 cosTable;
    /* 0x4 */ s32 sinTable;
    /* 0x8 */ s32 bitrev;
    /* 0xC */ s32 n;
} FFTTables;

void mdctFftCtx(s32 arg0, FFTTables *arg1, s32 arg2) {
    mdctFft(arg0, arg1->n, arg2, arg1->cosTable, arg1->sinTable, arg1->bitrev);
}

/* Round a float to the nearest s16, saturating at the s16 range. */
s16 adecRoundClampS16(f32 x) {
    if (x >= 0.0f) {
        x += 0.5f;
        if (x > 32767.0f) {
            x = 32767.0f;
        }
    } else {
        x -= 0.5f;
        if (x < -32768.0f) {
            x = -32768.0f;
        }
    }
    return x;
}

u32 adecStreamOpen(s32 *source, u32 divisor, s32 *first)
{
    DecodeState *state = &sAdecState;
    state->source = source;
    state->cursor = source;
    state->first = *(s32 *)state->cursor;
    state->cursor = (s32 *)state->cursor + 1;
    state->count = *(s32 *)state->cursor;
    state->second = *(s32 *)state->cursor;
    state->cursor = (s32 *)state->cursor + 1;
    state->third = *(s32 *)state->cursor;
    state->cursor = (s32 *)state->cursor + 1;
    sAdecHeaderDone = 0;
    sAdecImdctReady = 0;
    sAdecBitsLeft = 0;
    sAdecHuffBits = 0;
    sAdecHuffBitsAvail = 0;
    *first = state->first;
    return (u64)((s64)state->count * 0xF424000) / divisor;
}

void *adecNextFrame(void) {
    struct DecodeState *base = &sAdecState;
    unsigned short *counter;
    char *slots;
    unsigned short temp;
    if (base->count == 0) {
        return 0;
    }
    adecDecodeFrame(base);
    counter = &sAdecFrameCounter;
    temp = *counter + 1;
    *counter = temp;
    if ((unsigned int)(temp & 0xFFFF) >= 6) {
        *counter = 0;
    }
    slots = (char *) sAdecOutputFrames;
    adecImdctFrame(slots + (*counter << 9), base);
    base->count = base->count - 1;
    return slots + (*counter << 9);
}

#include "m2c_macros.h"

s32 adecGetStopFlag(void) {
    return sAdecStopFlag;
}

void adecClearStopFlag(void) {
    sAdecStopFlag = 0;
}
