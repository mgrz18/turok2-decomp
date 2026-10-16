/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B8370_5.c, func_802B35E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef int (*ALSetParam)(void *, int, void *);
struct ListLink;
struct ListLink {
    struct ListLink *next;
    struct ListLink *prev;
};
struct ListLink;
typedef struct ListLink ListLink;
struct ALSynth_func_802B3000_de;
typedef struct ALSynth_func_802B3000_de ALSynth_func_802B3000_de;
struct ALVoice_s_func_802B3000_de;
typedef struct ALVoice_s_func_802B3000_de ALVoice_s_func_802B3000_de;
struct ALSynth_func_802B3000_de;
struct ALSynth_func_802B3000_de {
    void *head;
    ListLink pFreeList;
    ListLink pAllocList;
    ListLink pLameList;
    s32 paramSamples;
};
struct Node_func_80239AF4_de;
typedef struct Node_func_80239AF4_de Node_func_80239AF4_de;
struct Node_func_80239AF4_de;
struct Node_func_80239AF4_de {
    struct Node_func_80239AF4_de *next;
};
struct ALFilter_s_func_802B3000_de;
struct ALFilter_s_func_802B3000_de {
    struct ALFilter_s_func_802B3000_de *source;
    void *handler;
    ALSetParam setParam;
};
struct ALFilter_s_func_802B3000_de;
struct ALVoice_s_func_802B3000_de;
struct PVoice_s_func_802B3000_de;
struct PVoice_s_func_802B3000_de {
    ListLink node;
    struct ALVoice_s_func_802B3000_de *vvoice;
    struct ALFilter_s_func_802B3000_de *channelKnob;
    char pad10[0xD8 - 0x10];
    s32 offset;
};
struct ALVoice_s_func_802B3000_de {
    ListLink node;
    struct PVoice_s_func_802B3000_de *pvoice;
    void *table;
    void *clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
};
struct ALFilter_s_func_802B3000_de;
typedef struct ALFilter_s_func_802B3000_de ALFilter_s_func_802B3000_de;
struct ALStartParamAlt;
struct Node_func_80239AF4_de;
struct ALStartParamAlt {
    struct Node_func_80239AF4_de *next;
    s32 delta;
    s16 type;
    s16 unity;
    f32 pitch;
    s16 volume;
    u8 pan;
    u8 fxMix;
    s32 samples;
    void *wave;
};
struct ALStartParamAlt;
typedef struct ALStartParamAlt ALStartParamAlt;
extern Node_func_80239AF4_de *func_00294B58(void);
extern s32 func_00294C30(ALSynth_func_802B3000_de *, s32);
void func_00294540(ALSynth_func_802B3000_de *s, ALVoice_s_func_802B3000_de *v, void *w, f32 pitch, s16 vol, u8 pan, s8 fxmix, s32 t)
{
    ALStartParamAlt *update;
    ALFilter_s_func_802B3000_de *f;
    if (v->pvoice) {
        update = (ALStartParamAlt *)func_00294B58();
        if (update == 0) { return; };
        if (fxmix < 0) {
            fxmix = -fxmix;
        }
        update->delta = s->paramSamples + v->pvoice->offset;
        update->next = 0;
        update->type = 13;
        update->unity = v->unityPitch;
        update->pan = pan;
        update->volume = vol;
        update->fxMix = fxmix;
        update->pitch = pitch;
        update->samples = func_00294C30(s, t);
        update->wave = w;
        f = v->pvoice->channelKnob;
        (*f->setParam)(f, 3, update);
    }
}
