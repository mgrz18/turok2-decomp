/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/code_802BAC64.c, func_802B5B94_de.
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
typedef signed long long s64;
typedef float f32;
struct Clip;
struct Clip {
    s16 mode;
    s16 frames;
};
struct Clip;
typedef struct Clip Clip;
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
struct func_8022E694_S1;
typedef struct func_8022E694_S1 func_8022E694_S1;
struct func_8022E694_S1;
struct func_8022E694_S1 {
    char pad0[0x44];
    s32 unk44;
};
extern const double D_800AA700;
extern const float D_800AA708;
typedef signed short AudioPoleFilterState[4];
struct AudioLowPassFilter;
struct AudioLowPassFilter {
    s16 cutoff;
    s16 gain;
    union {
        s16 taps[16];
        s64 alignment;
    } coefficients;
    AudioPoleFilterState *state;
    s32 first;
};
struct AudioLowPassFilter;
typedef struct AudioLowPassFilter AudioLowPassFilter;
struct ALDelay_func_802B5B94_de;
typedef struct ALDelay_func_802B5B94_de ALDelay_func_802B5B94_de;
struct ALFxCode802BAC64;
typedef struct ALFxCode802BAC64 ALFxCode802BAC64;
struct ALGlobals_func_802B5B94_de;
typedef struct ALGlobals_func_802B5B94_de ALGlobals_func_802B5B94_de;
struct ALDelay_func_802B5B94_de;
struct ALDelay_func_802B5B94_de {
    u32 input;
    u32 output;
    s16 ffcoef;
    s16 fbcoef;
    s16 gain;
    f32 rsinc;
    f32 rsval;
    s32 rsdelta;
    f32 rsgain;
    Clip *lp;
    void *rs;
};
struct ALDelay_func_802B5B94_de;
struct ALFxCode802BAC64;
struct ALFxCode802BAC64 {
    ALFilter_s14 filter;
    s16 *base;
    s16 *input;
    u32 length;
    struct ALDelay_func_802B5B94_de *delay;
    u8 section_count;
    void *paramHdl;
};
struct ALGlobals_func_802B5B94_de;
struct ALGlobals_func_802B5B94_de {
    func_8022E694_S1 drvr;
};
extern void func_002955EC(AudioLowPassFilter *lp);
extern float D_800AA6F8;
extern ALGlobals_func_802B5B94_de *D_800B7760;
s32 func_00296AF4(ALFxCode802BAC64 *filter, s32 paramID, void *param)
{
    ALFxCode802BAC64 *f = filter;
    s32 p = (paramID - 2) % 8;
    s32 s = (paramID - 2) / 8;
    s32 val = *(s32 *)param;
    switch (p) {
        case 0:
            f->delay[s].input = (u32)val & 0xFFFFFFF8;
            break;
        case 1:
            f->delay[s].output = (u32)val & 0xFFFFFFF8;
            break;
        case 3:
            f->delay[s].ffcoef = (s16)val;
            break;
        case 2:
            f->delay[s].fbcoef = (s16)val;
            break;
        case 4:
            f->delay[s].gain = (s16)val;
            break;
        case 5:
            f->delay[s].rsinc = (((f32)val * D_800AA6F8) * 2.0f) / D_800B7760->drvr.unk44;
            break;
        case 6: {
            f32 fval = (f32)val;
            s32 length = (f->delay[s].output - f->delay[s].input);
            double flength = length;
            if (length < 0)
                flength += D_800AA700;
            f->delay[s].rsgain = fval * ((f32)flength * D_800AA708);
            break;
        }
        case 7:
            if (f->delay[s].lp) {
                f->delay[s].lp->mode = (s16)val;
                func_002955EC(f->delay[s].lp);
            }
            break;
    }
    return 0;
}
