/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/code_802BAC64.c, func_802B5D70_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct {
    unsigned int w0;
    unsigned int w1;
} Awords;
typedef union {
    Awords words;
    long long int force_union_align;
} Acmd;
typedef Acmd *(*ALCmdHandler)(void *, short *, int, int, Acmd *);
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct ALFilter_s14;
typedef struct ALFilter_s14 ALFilter_s14;
struct ALFilter_s14;
struct ALFilter_s14 {
    struct ALFilter_s14 *source;
    ALCmdHandler handler;
    void *setParam;
    s16 inp;
    s16 outp;
    s32 type;
};
struct ALDelay28;
typedef struct ALDelay28 ALDelay28;
struct ALResampler_s28;
struct ALResampler_s28 {
    ALFilter_s14 filter;
    void *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
};
struct ALResampler_s28;
typedef struct ALResampler_s28 ALResampler_s28;
struct ALDelay28;
struct ALDelay28 {
    u32 input;
    u32 output;
    s16 ffcoef;
    s16 fbcoef;
    s16 gain;
    f32 rsinc;
    f32 rsval;
    s32 rsdelta;
    f32 rsgain;
    void *lp;
    ALResampler_s28 *rs;
};
struct ALFx;
typedef struct ALFx ALFx;
struct ALFx;
struct ALFx {
    ALFilter_s14 filter;
    s16 *base;
    s16 *input;
    u32 length;
    ALDelay28 *delay;
    u8 section_count;
    void *paramHdl;
};
extern f32 func_00297260(ALDelay28 *d, s32 count);
extern Acmd *func_0029704C(ALFx *, s16 *, s32, s32, Acmd *);
extern float D_800AA710;
extern float D_800AA714;
extern float D_800AA70C;
extern const f32 D_800AA70C;
extern const f32 D_800AA710;
extern const f32 D_800AA714;
Acmd *func_00296CD0(ALFx *r, ALDelay28 *d, s32 buff, s32 incount, Acmd *p)
{
    extern s32 func_0029BF80(s32);
    Acmd *ptr = p;
    s32 ratio, count, rbuff = 640;
    s16 *out_ptr;
    f32 fincount, fratio, delta;
    s32 ramalign = 0, length;
    f32 unity;
    if (d->rs) {
        length = d->output - d->input;
        delta = func_00297260(d, incount);
        delta /= length;
        unity = D_800AA70C;
        delta = (s32)(delta * unity);
        delta = delta * D_800AA710;
        fratio = D_800AA714 - delta;
        fincount = d->rs->delta + (fratio * (f32)incount);
        count = (s32)fincount;
        d->rs->delta = fincount - (f32)count;
        out_ptr = &r->input[-(d->output - d->rsdelta)];
        ramalign = ((s32)out_ptr & 0x7) >> 1;
        ptr = func_0029704C(r, out_ptr - ramalign, rbuff, count + ramalign, ptr);
        ratio = (s32)(fratio * unity);
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(rbuff + (ramalign << 1)) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(buff) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(incount << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(d->rs->first) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(ratio) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (unsigned int)(func_0029BF80((s32)d->rs->state)); };
        d->rs->first = 0;
        d->rsdelta += count - incount;
    } else {
        out_ptr = &r->input[-d->output];
        ptr = func_0029704C(r, out_ptr, buff, incount, ptr);
    }
    return ptr;
}
Acmd *func_0029704C(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
f32 func_00297260(ALDelay28 *d, s32 count);
