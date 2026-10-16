/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/code_802BAC64.c, func_802B60EC_de.
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
extern Acmd *func_0029704C(ALFx *, s16 *, s32, s32, Acmd *);
Acmd *func_0029704C(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p)
{
    extern u32 func_0029BF80(void *);
    Acmd *ptr = p;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;
    delay_end = &r->base[r->length];
    if (curr_ptr < r->base)
        curr_ptr += r->length;
    updated_ptr = curr_ptr + count;
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(buff) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(before_end << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(func_0029BF80(curr_ptr)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(buff + (before_end << 1)) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(after_end << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(func_0029BF80(r->base)); };
    } else {
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(buff) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(count << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(func_0029BF80(curr_ptr)); };
    }
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(count << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    return ptr;
}
