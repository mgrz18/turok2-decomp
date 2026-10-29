/* From decompals/ultralib (libultra 2.0I): src/audio/synthesizer.c, alSynNew.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef short s16;
typedef long s32;
typedef long long s64;
typedef float f32;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int gain:16;
 unsigned int addr;
} Aadpcm;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int gain:16;
 unsigned int addr;
} Apolef;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int pad1:16;
 unsigned int addr;
} Aenvelope;
typedef struct {
   unsigned int cmd:8;
 unsigned int pad1:8;
 unsigned int dmem:16;
 unsigned int pad2:16;
 unsigned int count:16;
} Aclearbuff;
typedef struct {
   unsigned int cmd:8;
 unsigned int pad1:8;
 unsigned int pad2:16;
 unsigned int inL:16;
        unsigned int inR:16;
} Ainterleave;
typedef struct {
   unsigned int cmd:8;
 unsigned int pad1:24;
 unsigned int addr;
} Aloadbuff;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int pad1:16;
 unsigned int addr;
} Aenvmixer;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int gain:16;
 unsigned int dmemi:16;
 unsigned int dmemo:16;
} Amixer;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int pitch:16;
 unsigned int addr;
} Aresample;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int pad1:16;
 unsigned int addr;
} Areverb;
typedef struct {
   unsigned int cmd:8;
 unsigned int pad1:24;
 unsigned int addr;
} Asavebuff;
typedef struct {
   unsigned int cmd:8;
 unsigned int pad1:24;
 unsigned int pad2:2;
 unsigned int number:4;
 unsigned int base:24;
} Asegment;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int dmemin:16;
 unsigned int dmemout:16;
 unsigned int count:16;
} Asetbuff;
typedef struct {
   unsigned int cmd:8;
 unsigned int flags:8;
 unsigned int vol:16;
 unsigned int voltgt:16;
 unsigned int volrate:16;
} Asetvol;
typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:8;
    unsigned int dmemin:16;
    unsigned int dmemout:16;
    unsigned int count:16;
} Admemmove;
typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:8;
    unsigned int count:16;
    unsigned int addr;
} Aloadadpcm;
typedef struct {
    unsigned int cmd:8;
    unsigned int pad1:8;
    unsigned int pad2:16;
    unsigned int addr;
} Asetloop;
typedef struct {
 unsigned int w0;
 unsigned int w1;
} Awords;
typedef union {
 Awords words;
 Aadpcm adpcm;
        Apolef polef;
 Aclearbuff clearbuff;
 Aenvelope envelope;
        Ainterleave interleave;
 Aloadbuff loadbuff;
        Aenvmixer envmixer;
 Aresample resample;
 Areverb reverb;
 Asavebuff savebuff;
 Asegment segment;
 Asetbuff setbuff;
 Asetvol setvol;
        Admemmove dmemmove;
        Aloadadpcm loadadpcm;
        Amixer mixer;
        Asetloop setloop;
        long long int force_union_align;
} Acmd;
typedef short ADPCM_STATE[16];
typedef short POLEF_STATE[4];
typedef short RESAMPLE_STATE[16];
typedef short ENVMIX_STATE[40];
typedef s32 ALMicroTime;
typedef struct ALLink_s {
    struct ALLink_s *next;
    struct ALLink_s *prev;
} ALLink;
void func_002933E0(ALLink *element, ALLink *after);
typedef s32 (*ALDMAproc)(s32 addr, s32 len, void *state);
typedef ALDMAproc (*ALDMANew)(void *state);
typedef struct {
    u8 *base;
    u8 *cur;
    s32 len;
    s32 count;
} ALHeap;
void *func_002912A0(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
typedef u8 ALFxId;
typedef void *ALFxRef;
typedef struct {
    s32 order;
    s32 npredictors;
    s16 book[1];
} ALADPCMBook;
typedef struct {
    u32 start;
    u32 end;
    u32 count;
    ADPCM_STATE state;
} ALADPCMloop;
typedef struct {
    u32 start;
    u32 end;
    u32 count;
} ALRawLoop;
typedef struct {
    ALADPCMloop *loop;
    ALADPCMBook *book;
} ALADPCMWaveInfo;
typedef struct {
    ALRawLoop *loop;
} ALRAWWaveInfo;
typedef struct ALWaveTable_s {
    u8 *base;
    s32 len;
    u8 type;
    u8 flags;
    union {
        ALADPCMWaveInfo adpcmWave;
        ALRAWWaveInfo rawWave;
    } waveInfo;
} ALWaveTable;
typedef ALMicroTime (*ALVoiceHandler)(void *);
typedef struct {
    s32 maxVVoices;
    s32 maxPVoices;
    s32 maxUpdates;
    s32 maxFXbusses;
    void *dmaproc;
    ALHeap *heap;
    s32 outputRate;
    ALFxId fxType;
    s32 *params;
} ALSynConfig;
typedef struct ALPlayer_s {
    struct ALPlayer_s *next;
    void *clientData;
    ALVoiceHandler handler;
    ALMicroTime callTime;
    s32 samplesLeft;
} ALPlayer;
typedef struct ALVoice_s {
    ALLink node;
    struct PVoice_s *pvoice;
    ALWaveTable *table;
    void *clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
} ALVoice;
typedef struct {
    ALPlayer *head;
    ALLink pFreeList;
    ALLink pAllocList;
    ALLink pLameList;
    s32 paramSamples;
    s32 curSamples;
    ALDMANew dma;
    ALHeap *heap;
    struct ALParam_s *paramList;
    struct ALMainBus_s *mainBus;
    struct ALAuxBus_s *auxBus;
    struct ALFilter_s *outputFilter;
    s32 numPVoices;
    s32 maxAuxBusses;
    s32 outputRate;
    s32 maxOutSamples;
} ALSynth;
void alSynNew(ALSynth *s, ALSynConfig *config);
ALFxRef *func_00297420(ALSynth *s, s16 bus, ALSynConfig *c, ALHeap *hp);
enum {
    AL_FILTER_FREE_VOICE,
    AL_FILTER_SET_SOURCE,
    AL_FILTER_ADD_SOURCE,
    AL_FILTER_ADD_UPDATE,
    AL_FILTER_RESET,
    AL_FILTER_SET_WAVETABLE,
    AL_FILTER_SET_DRAM,
    AL_FILTER_SET_PITCH,
    AL_FILTER_SET_UNITY_PITCH,
    AL_FILTER_START,
    AL_FILTER_SET_STATE,
    AL_FILTER_SET_VOLUME,
    AL_FILTER_SET_PAN,
    AL_FILTER_START_VOICE_ALT,
    AL_FILTER_START_VOICE,
    AL_FILTER_STOP_VOICE,
    AL_FILTER_SET_FXAMT
};
typedef struct ALParam_s {
    struct ALParam_s *next;
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
    union {
        f32 f;
        s32 i;
    } stillmoredata;
    union {
        f32 f;
        s32 i;
    } yetstillmoredata;
} ALParam;
typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);
typedef s32 (*ALSetParam)(void *, s32, void *);
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    ALCmdHandler handler;
    ALSetParam setParam;
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
typedef struct {
    ALFilter filter;
    ADPCM_STATE *state;
    ADPCM_STATE *lstate;
    ALRawLoop loop;
    struct ALWaveTable_s *table;
    s32 bookSize;
    ALDMAproc dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    s32 memin;
} ALLoadFilter;
void func_00295304(ALLoadFilter *f, ALDMANew dma, ALHeap *hp);
s32 func_0029FB64(void *filter, s32 paramID, void *param);
typedef struct ALResampler_s {
    ALFilter filter;
    RESAMPLE_STATE *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
    ALParam *ctrlList;
    ALParam *ctrlTail;
    s32 motion;
} ALResampler;
typedef struct {
    s16 fc;
    s16 fgain;
    union {
        s16 fccoef[16];
        s64 force_aligned;
    } fcvec;
    POLEF_STATE *fstate;
    s32 first;
} ALLowPass;
typedef struct {
    u32 input;
    u32 output;
    s16 ffcoef;
    s16 fbcoef;
    s16 gain;
    f32 rsinc;
    f32 rsval;
    s32 rsdelta;
    f32 rsgain;
    ALLowPass *lp;
    ALResampler *rs;
} ALDelay;
typedef s32 (*ALSetFXParam)(void *, s32, void *);
typedef struct {
    struct ALFilter_s filter;
    s16 *base;
    s16 *input;
    u32 length;
    ALDelay *delay;
    u8 section_count;
    ALSetFXParam paramHdl;
} ALFx;
typedef struct ALMainBus_s {
    ALFilter filter;
    s32 sourceCount;
    s32 maxSources;
    ALFilter **sources;
} ALMainBus;
void func_002953B4(ALMainBus *m, void *ptr, s32 len);
s32 func_002964A0(void *filter, s32 paramID, void *param);
typedef struct ALAuxBus_s {
    ALFilter filter;
    s32 sourceCount;
    s32 maxSources;
    ALFilter **sources;
    ALFx fx[1];
} ALAuxBus;
void func_00295410(ALAuxBus *m, void *ptr, s32 len);
s32 func_00294E3C(void *filter, s32 paramID, void *param);
void func_0029546C(ALResampler *r, ALHeap *hp);
s32 func_00296690(void *f, s32 paramID, void *param);
typedef struct ALSave_s {
    ALFilter filter;
    s32 dramout;
    s32 first;
} ALSave;
void func_002954F8(ALSave *r);
s32 func_002973E0(void *f, s32 paramID, void *param);
typedef struct ALEnvMixer_s {
    ALFilter filter;
    ENVMIX_STATE *state;
    s16 pan;
    s16 volume;
    s16 cvolL;
    s16 cvolR;
    s16 dryamt;
    s16 wetamt;
    u16 lratl;
    s16 lratm;
    s16 ltgt;
    u16 rratl;
    s16 rratm;
    s16 rtgt;
    s32 delta;
    s32 segEnd;
    s32 first;
    ALParam *ctrlList;
    ALParam *ctrlTail;
    ALFilter **sources;
    s32 motion;
} ALEnvMixer;
void func_0029553C(ALEnvMixer *e, ALHeap *hp);
s32 func_00296118(void *filter, s32 paramID, void *param);
typedef struct PVoice_s {
    ALLink node;
    struct ALVoice_s *vvoice;
    ALFilter *channelKnob;
    ALLoadFilter decoder;
    ALResampler resampler;
    ALEnvMixer envmixer;
    s32 offset;
} PVoice;
void alSynNew(ALSynth *drvr, ALSynConfig *c)
{
    s32 i;
    ALVoice *vv;
    PVoice *pv;
    ALVoice *vvoices;
    PVoice *pvoices;
    ALHeap *hp = c->heap;
    ALSave *save;
    ALFilter *sources;
    ALParam *params;
    ALParam *paramPtr;
    drvr->head = 0;
    drvr->numPVoices = c->maxPVoices;
    drvr->curSamples = 0;
    drvr->paramSamples = 0;
    drvr->outputRate = c->outputRate;
    drvr->maxOutSamples = 160;
    drvr->dma = (ALDMANew) c->dmaproc;
    save = func_002912A0(0, 0,(hp),(1),(sizeof(ALSave)));
    func_002954F8(save);
    drvr->outputFilter = (ALFilter *)save;
    drvr->auxBus = func_002912A0(0, 0,(hp),(1),(sizeof(ALAuxBus)));
    drvr->maxAuxBusses = 1;
    sources = func_002912A0(0, 0,(hp),(c->maxPVoices),(sizeof(ALFilter *)));
    func_00295410(drvr->auxBus, sources, c->maxPVoices);
    drvr->mainBus = func_002912A0(0, 0,(hp),(1),(sizeof(ALMainBus)));
    sources = func_002912A0(0, 0,(hp),(c->maxPVoices),(sizeof(ALFilter *)));
    func_002953B4(drvr->mainBus, sources, c->maxPVoices);
    if (c->fxType != 0){
        func_00297420(drvr, 0, c, hp);
    } else
     func_002964A0(drvr->mainBus, AL_FILTER_ADD_SOURCE, &drvr->auxBus[0]);
    drvr->pFreeList.next = 0;
    drvr->pFreeList.prev = 0;
    drvr->pLameList.next = 0;
    drvr->pLameList.prev = 0;
    drvr->pAllocList.next = 0;
    drvr->pAllocList.prev = 0;
    pvoices = func_002912A0(0, 0,(hp),(c->maxPVoices),(sizeof(PVoice)));
    for (i = 0; i < c->maxPVoices; i++) {
        pv = &pvoices[i];
        func_002933E0((ALLink *)pv, &drvr->pFreeList);
        pv->vvoice = 0;
        func_00295304(&pv->decoder, drvr->dma, hp);
        func_0029FB64(&pv->decoder, AL_FILTER_SET_SOURCE, 0);
        func_0029546C(&pv->resampler, hp);
        func_00296690(&pv->resampler, AL_FILTER_SET_SOURCE, &pv->decoder);
        func_0029553C(&pv->envmixer, hp);
        func_00296118(&pv->envmixer, AL_FILTER_SET_SOURCE, &pv->resampler);
        func_00294E3C(drvr->auxBus, AL_FILTER_ADD_SOURCE, &pv->envmixer);
        pv->channelKnob = (ALFilter *)&pv->envmixer;
    }
    func_002973E0(save, AL_FILTER_SET_SOURCE, drvr->mainBus);
    params = func_002912A0(0, 0,(hp),(c->maxUpdates),(sizeof(ALParam)));
    drvr->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr= &params[i];
        paramPtr->next = drvr->paramList;
        drvr->paramList = paramPtr;
    }
    drvr->heap = hp;
}
