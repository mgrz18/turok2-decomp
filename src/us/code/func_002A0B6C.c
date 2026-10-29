/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/adecReadBits.c, adecImdctFrame, with Turok 2's layouts. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */

#include "common.h"

/* Acclaim compressed-audio stream decoder state (one instance) */
typedef struct {
    /* 0x00 */ u32 hdr0;
    /* 0x04 */ u32 hdr2;
    /* 0x08 */ u32 *hdr;
    /* 0x0C */ u32 nframes;
    /* 0x10 */ u32 remaining;
    /* 0x14 */ u16 *rd;         /* bitstream read pointer */
    /* 0x18 */ s16 band[16];    /* per-band scale indices, followed by the coefficients at 0xB8 */
} AdecStream;

typedef struct {
    f32 re;
    f32 im;
} AdecCpx;

/* FFT tables: n complex points */
typedef struct {
    /* 0x00 */ f32 *cos;
    /* 0x04 */ f32 *sin;
    /* 0x08 */ s32 *bitrev;
    /* 0x0C */ s32 n;
} MdctCtx;

extern s16 D_800B8BB0;          /* IMDCT tables built */
extern MdctCtx *D_80125A30;     /* FFT context */
extern f32 D_80125030[128];     /* pre-twiddle cos */
extern f32 D_80125230[128];     /* pre-twiddle sin */
extern f32 D_80125430[128];     /* post-twiddle cos + sin */
extern f32 D_80125630[128];     /* post-twiddle sin - cos */
extern f32 D_80125830[128];     /* overlap state */

extern void func_002A05D4(void);                    /* mdctInitTwiddles */
extern MdctCtx *func_002A073C(s32 n);               /* mdctFftInit */
extern s32 func_002A08C4(f32 *out, s16 *band);      /* adecDequantize */
extern void func_002A09E8(f32 *d, s32 n, s32 sign, f32 *cos, f32 *sin, s32 *bitrev); /* mdctFft */

/* out[k] = (in[2k], in[255 - 2k]) for k < 128: folds the 256 spectrum values into 128 complex points. */
static __inline__ void mdctPackSpectrum(AdecCpx *out, f32 *in) {
    s16 k;
    for (k = 0; k < 128; k++) {
        out[k].re = in[k * 2];
        out[k].im = in[255 - k * 2];
    }
}

/* d[k] *= (c[k] + i s[k]) for the 128 complex points. */
static __inline__ void mdctTwiddleMul(AdecCpx *d, f32 *c, f32 *s) {
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

/* FFT with the tables of an MdctCtx. */
static __inline__ void mdctFftCtx(f32 *data, MdctCtx *ctx, s32 sign) {
    func_002A09E8(data, ctx->n, sign, ctx->cos, ctx->sin, ctx->bitrev);
}

/* Rounds to nearest (half away from zero) and clamps to the s16 range. */
static __inline__ s16 adecRoundClampS16(f32 x) {
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
void func_002A0B6C(s16 *out, AdecStream *st) {
    f32 buf[256];
    f32 deq[256];
    s16 k;
    f32 scale;
    if (D_800B8BB0 == 0) {
        func_002A05D4();
        D_80125A30 = func_002A073C(0x80);
        D_800B8BB0 = 1;
    }
    scale = 23170.475f;
    func_002A08C4(deq, st->band);
    mdctPackSpectrum((AdecCpx *)buf, deq);
    mdctTwiddleMul((AdecCpx *)buf, D_80125030, D_80125230);
    mdctFftCtx(buf, D_80125A30, -1);
    mdctTwiddleMul((AdecCpx *)buf, D_80125430, D_80125630);
    for (k = 0; k < 128; k++) {
        f32 ov = D_80125830[k];
        f32 cc = D_80125030[k];
        f32 im = buf[k * 2 + 1];
        f32 ss = D_80125230[k];
        f32 t1 = ov * cc + im * ss;
        f32 t2 = ov * ss - im * cc;
        D_80125830[k] = buf[k * 2];
        out[k * 2] = adecRoundClampS16(scale * t1);
        out[255 - k * 2] = adecRoundClampS16(scale * t2);
    }
}
