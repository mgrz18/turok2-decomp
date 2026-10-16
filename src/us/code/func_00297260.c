/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/code_802BAC64.c, func_802B6300_de.
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
extern f32 func_00297260(ALDelay28 *d, s32 count);
extern const float D_800AA718;
extern const float D_800AA720;
f32 func_00297260(ALDelay28 *d, s32 count)
{
    f32 val;
    d->rsval += d->rsinc * count;
    d->rsval = (d->rsval > D_800AA718) ? d->rsval - (*(&D_800AA718 + 1)) : d->rsval;
    val = d->rsval;
    val = (val < 0) ? -val : val;
    val -= D_800AA720;
    return d->rsgain * val;
}
