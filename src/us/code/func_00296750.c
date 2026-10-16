/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B53FC_de_padding_B5FFC_12.c, func_802B57F0_de.
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
typedef unsigned short u16;
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
struct ALFilter_s14_2;
typedef struct ALFilter_s14_2 ALFilter_s14_2;
struct ALFilter_s14_2;
struct ALFilter_s14_2 {
    struct ALFilter_s14_2 *source;
    ALCmdHandler handler;
    void *setParam;
    s16 inp;
    s16 outp;
    s32 type;
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
extern void *func_002971BC(void *arg0, s32 arg1, s32 arg2, void *arg3);
extern Acmd *func_0029704C(ALFx *, s16 *, s32, s32, Acmd *);
extern char D_800AA6C0[];
extern char D_800AA6C4[];
extern void func_0029B020(char *, char *, s32);
extern Acmd *func_00296CD0(struct ALFx *, ALDelay28 *, s32, s32, Acmd *);
extern Acmd *func_0029704C(struct ALFx *, s16 *, s32, s32, Acmd *);
extern Acmd *func_00296EC8(struct ALFx *, s16 *, s32, s32, Acmd *);
Acmd *func_00296750(struct ALFx *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    struct ALFx *r = filter;
    ALFilter_s14_2 *source = (ALFilter_s14_2 *)r->filter.source;
    s16 i, buff1, buff2, input, output;
    s16 *in_ptr, *out_ptr, gain, *prev_out_ptr = 0;
    ALDelay28 *d, *pd;
    if (!source)
        func_0029B020(D_800AA6C0, D_800AA6C4, 75);
    ptr = (*source->handler)(source, outp, outCount, sampleOffset, p);
    input = 1728;
    output = 2048;
    buff1 = 0;
    buff2 = 320;
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(outCount << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0xda83) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(1728) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(input) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0x5a82) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(2048) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(input) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    ptr = func_00296EC8(r, r->input, input, outCount, ptr);
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(output) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); _a->words.w1 = (unsigned int)(outCount << 1); };
    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        in_ptr = &r->input[-d->input];
        out_ptr = &r->input[-d->output];
        if (in_ptr == prev_out_ptr) {
            { s16 t = buff2; buff2 = buff1; buff1 = t; };
        } else {
            ptr = func_0029704C(r, in_ptr, buff1, outCount, ptr);
        }
        ptr = func_00296CD0(r, d, buff2, outCount, ptr);
        if (d->ffcoef) {
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)((u16)d->ffcoef) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(buff1) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(buff2) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
            if (!d->rs && !d->lp) {
                ptr = func_00296EC8(r, out_ptr, buff2, outCount, ptr);
            }
        }
        if (d->fbcoef) {
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)((u16)d->fbcoef) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(buff2) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(buff1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
            ptr = func_00296EC8(r, in_ptr, buff1, outCount, ptr);
        }
        if (d->lp)
            ptr = func_002971BC(d->lp, buff2, outCount, ptr);
        if (!d->rs)
            ptr = func_00296EC8(r, out_ptr, buff2, outCount, ptr);
        if (d->gain)
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)((u16)d->gain) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(buff2) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(output) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        prev_out_ptr = &r->input[d->output];
    }
    r->input += outCount;
    if (r->input > &r->base[r->length])
        r->input -= r->length;
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(10) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(output) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); _a->words.w1 = (((unsigned int)(1728) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(outCount << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    return ptr;
}
