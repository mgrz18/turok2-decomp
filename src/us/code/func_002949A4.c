/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_12.c, func_802B3A44_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef struct {
    unsigned int w0;
    unsigned int w1;
} Awords;
typedef union {
    Awords words;
    long long int force_union_align;
} Acmd;
typedef Acmd *(*ALCmdHandler)(void *, short *, int, int, Acmd *);
typedef int (*ALVoiceHandler)(void *);
typedef int (*ALSetParam)(void *, int, void *);
struct ALPlayer_s14;
struct ALPlayer_s14 {
    struct ALPlayer_s14 *next;
    void *clientData;
    ALVoiceHandler handler;
    s32 callTime;
    s32 samplesLeft;
};
struct ALPlayer_s14;
typedef struct ALPlayer_s14 ALPlayer_s14;
struct ListLink;
struct ListLink {
    struct ListLink *next;
    struct ListLink *prev;
};
struct ListLink;
typedef struct ListLink ListLink;
struct ALFilter_sC;
struct ALFilter_sC {
    struct ALFilter_sC *source;
    ALCmdHandler handler;
    ALSetParam setParam;
};
struct ALFilter_sC;
typedef struct ALFilter_sC ALFilter_sC;
struct ALSynth4C;
struct ALSynth4C {
    ALPlayer_s14 *head;
    ListLink pFreeList;
    ListLink pAllocList;
    ListLink pLameList;
    s32 paramSamples;
    s32 curSamples;
    void *dma;
    void *heap;
    void *paramList;
    void *mainBus;
    void *auxBus;
    ALFilter_sC *outputFilter;
    s32 numPVoices;
    s32 maxAuxBusses;
    s32 outputRate;
    s32 maxOutSamples;
};
struct ALSynth4C;
typedef struct ALSynth4C ALSynth4C;
struct ALGlobals;
struct ALGlobals {
    ALSynth4C drvr;
};
struct ALGlobals;
typedef struct ALGlobals ALGlobals;
extern s32 func_00294C70(ALSynth4C *drvr, ALPlayer_s14 **client);
extern s32 func_00294D18(ALSynth4C *, s32);
extern void func_00294BDC(ALSynth4C *);
Acmd *func_002949A4(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen)
{
    extern ALGlobals *D_800B7760;
    ALPlayer_s14 *client;
    ALFilter_sC *output;
    ALSynth4C *drvr = &D_800B7760->drvr;
    s16 tmp = 0;
    Acmd *cmdlEnd = cmdList;
    Acmd *cmdPtr;
    s32 nOut;
    s16 *lOutBuf = outBuf;
    if (drvr->head == 0) {
        *cmdLen = 0;
        return cmdList;
    }
    for (drvr->paramSamples = func_00294C70(drvr, &client);
         drvr->paramSamples - drvr->curSamples < outLen;
         drvr->paramSamples = func_00294C70(drvr, &client)) {
        drvr->paramSamples &= ~0xf;
        client->samplesLeft += func_00294D18(drvr, (*client->handler)(client));
    }
    drvr->paramSamples &= ~0xf;
    while (outLen > 0) {
        nOut = ((drvr->maxOutSamples) < (outLen) ? (drvr->maxOutSamples) : (outLen));
        cmdPtr = cmdlEnd;
        { Acmd *_a = (Acmd *)(cmdPtr++); _a->words.w0 = (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); };
        output = drvr->outputFilter;
        (*output->setParam)(output, 6, lOutBuf);
        cmdlEnd = (*output->handler)(output, &tmp, nOut, drvr->curSamples, cmdPtr);
        outLen -= nOut;
        lOutBuf += nOut << 1;
        drvr->curSamples += nOut;
    }
    *cmdLen = (s32)(cmdlEnd - cmdList);
    func_00294BDC(drvr);
    return cmdlEnd;
}
