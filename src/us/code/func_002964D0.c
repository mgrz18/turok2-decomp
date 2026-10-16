/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B53FC_de_padding_B5FFC_12.c, func_802B5570_de.
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
typedef signed short s16;
typedef signed int s32;
typedef float f32;
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
struct ALResampler_s;
struct ALResampler_s {
    ALFilter_s14_2 filter;
    void *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
    void *ctrlList;
    void *ctrlTail;
    s32 motion;
};
struct ALResampler_s;
typedef struct ALResampler_s ALResampler_s;
extern float D_800AA688;
extern float D_800AA680;
extern float D_800AA684;
extern unsigned int func_0029BF80(void *);
Acmd *func_002964D0(ALResampler_s *filter, s16 *outp, s32 outCnt, s32 sampleOffset, Acmd *p)
{
    ALResampler_s *f = filter;
    Acmd *ptr = p;
    s16 inp;
    s32 inCount;
    ALFilter_s14_2 *source = f->filter.source;
    s32 incr;
    f32 finCount;
    f32 ratio;
    f32 unity;
    inp = 320;
    if (!outCnt)
        return ptr;
    if (f->upitch) {
        ptr = (*source->handler)(source, &inp, outCnt, sampleOffset, p);
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(10) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(inp) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); _a->words.w1 = (((unsigned int)(*outp) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(outCnt << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    } else {
        if (f->ratio > D_800AA680) f->ratio = D_800AA680;
        ratio = f->ratio;
        unity = D_800AA684;
        f->ratio = (f32)(s32)(ratio * unity) * D_800AA688;
        finCount = f->delta + (f->ratio * (f32)outCnt);
        inCount = (s32)finCount;
        f->delta = finCount - (f32)inCount;
        ptr = (*source->handler)(source, &inp, inCount, sampleOffset, p);
        incr = (s32)(f->ratio * unity);
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(inp) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(*outp) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(outCnt << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(f->first) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(incr) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (unsigned int)(func_0029BF80(f->state)); };
        f->first = 0;
    }
    return ptr;
}
