/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/synaddplayer.c, func_802B3000_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

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
struct ALVoiceConfig_s;
typedef struct ALVoiceConfig_s ALVoiceConfig_s;
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
struct ALVoiceConfig_s;
struct ALVoiceConfig_s {
    s16 priority;
    s16 fxBus;
    u8 unityPitch;
};
struct ALFilter_s_func_802B3000_de;
typedef struct ALFilter_s_func_802B3000_de ALFilter_s_func_802B3000_de;
struct ALParam_s_func_802B3000_de;
typedef struct ALParam_s_func_802B3000_de ALParam_s_func_802B3000_de;
struct PVoice_s_func_802B3000_de;
typedef struct PVoice_s_func_802B3000_de PVoice_s_func_802B3000_de;
struct ALParam_s_func_802B3000_de;
struct ALParam_s_func_802B3000_de {
    struct ALParam_s_func_802B3000_de *next;
    s32 delta;
    s16 type;
    union {
        f32 f;
        s32 i;
    } data;
    union {
        f32 f;
        s32 i;
    } moredata;
};
extern s32 func_00294090(ALSynth_func_802B3000_de *, PVoice_s_func_802B3000_de **, s16);
extern ALParam_s_func_802B3000_de *func_00294B58(void);
s32 alSynAllocVoice(ALSynth_func_802B3000_de *drvr, ALVoice_s_func_802B3000_de *voice, ALVoiceConfig_s *vc)
{
    PVoice_s_func_802B3000_de *pvoice = 0;
    ALFilter_s_func_802B3000_de *f;
    ALParam_s_func_802B3000_de *update;
    s32 stolen;
    voice->priority = vc->priority;
    voice->unityPitch = vc->unityPitch;
    voice->table = 0;
    voice->fxBus = vc->fxBus;
    voice->state = 0;
    voice->pvoice = 0;
    stolen = func_00294090(drvr, &pvoice, vc->priority);
    if (pvoice) {
        f = pvoice->channelKnob;
        if (stolen) {
            pvoice->offset = 512;
            pvoice->vvoice->pvoice = 0;
            update = func_00294B58();
            update->delta = drvr->paramSamples;
            update->type = 11;
            update->data.i = 0;
            update->moredata.i = pvoice->offset - 64;
            (*f->setParam)(f, 3, update);
            update = func_00294B58();
            if (update) {
                update->delta = drvr->paramSamples + pvoice->offset;
                update->type = 15;
                update->next = 0;
                (*f->setParam)(f, 3, update);
            } else {
            }
        } else {
            pvoice->offset = 0;
        }
        pvoice->vvoice = voice;
        voice->pvoice = pvoice;
    }
    return (pvoice != 0);
}
