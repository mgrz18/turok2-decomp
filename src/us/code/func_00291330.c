/* Boundary fixed in versions/cuts.us.txt: splat starts func_00291324 three words early (stray words); the
 * function starts at 0x291330. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/csplayer.c, alCSPNew, with Turok 2's layouts.
 * (decompals/ultralib src/audio/csplayer.c; Turok 2's player has a debugFlags
 * byte and three extra words cleared at 0x7C-0x84.) */

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef long s32;

typedef struct ALPlayer_s {
    /* 0x00 */ struct ALPlayer_s *next;
    /* 0x04 */ void *clientData;
    /* 0x08 */ s32 (*handler)(void *);
    /* 0x0C */ s32 callTime;
    /* 0x10 */ s32 samplesLeft;
} ALPlayer;

typedef struct ALVoiceState_s {
    /* 0x00 */ struct ALVoiceState_s *next;
    /* 0x04 */ u8 pad04[0x34];
} ALVoiceState; /* size 0x38 */

typedef struct {
    /* 0x00 */ s16 type;
    /* 0x04 */ s32 msg[3];
} ALEvent;

typedef struct {
    /* 0x00 */ void *freeList;
    /* 0x04 */ void *allocList[2];
    /* 0x0C */ s32 eventCount;
    /* 0x10 */ s32 pad10;
} ALEventQueue;

typedef struct {
    /* 0x00 */ s32 maxVoices;
    /* 0x04 */ s32 maxEvents;
    /* 0x08 */ u8 maxChannels;
    /* 0x09 */ u8 debugFlags;
    /* 0x0C */ void *heap;
    /* 0x10 */ void *initOsc;
    /* 0x14 */ void *updateOsc;
    /* 0x18 */ void *stopOsc;
} ALSeqpConfig;

typedef struct {
    /* 0x00 */ ALPlayer node;
    /* 0x14 */ void *drvr;
    /* 0x18 */ void *target;
    /* 0x1C */ s32 curTime;
    /* 0x20 */ void *bank;
    /* 0x24 */ s32 uspt;
    /* 0x28 */ s32 nextDelta;
    /* 0x2C */ s32 state;
    /* 0x30 */ u16 chanMask;
    /* 0x32 */ s16 vol;
    /* 0x34 */ u8 maxChannels;
    /* 0x35 */ u8 debugFlags;
    /* 0x38 */ ALEvent nextEvent;
    /* 0x48 */ ALEventQueue evtq;
    /* 0x5C */ s32 frameTime;
    /* 0x60 */ void *chanState;
    /* 0x64 */ ALVoiceState *vAllocHead;
    /* 0x68 */ ALVoiceState *vAllocTail;
    /* 0x6C */ ALVoiceState *vFreeList;
    /* 0x70 */ void *initOsc;
    /* 0x74 */ void *updateOsc;
    /* 0x78 */ void *stopOsc;
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ s32 unk84;
} ALCSPlayer;

#define AL_STOPPED 0
#define AL_USEC_PER_FRAME 16000
#define AL_SEQP_API_EVT 9

extern void *D_800B7760; /* alGlobals (its drvr is at offset 0) */
void *func_002912A0(char *file, s32 line, void *hp, s32 num, s32 size); /* alHeapDBAlloc */
void func_002928F0(ALCSPlayer *seqp);                                   /* __initChanState */
void func_00290F20(ALEventQueue *evtq, void *items, s32 itemCount);     /* alEvtqNew */
void func_00293F10(void *drvr, ALPlayer *client);                       /* alSynAddPlayer */
s32 func_002914B4(void *node);                                          /* __CSPVoiceHandler */

#define alHeapAlloc(hp, num, size) func_002912A0(0, 0, hp, num, size)

void func_00291330(ALCSPlayer *seqp, ALSeqpConfig *c) {
    s32 i;
    void *items;
    ALVoiceState *vs;
    ALVoiceState *voices;

    void *hp = c->heap;

    seqp->bank = 0;
    seqp->target = 0;
    seqp->drvr = D_800B7760;
    seqp->chanMask = 0xFF;
    seqp->uspt = 488;
    seqp->nextDelta = 0;
    seqp->state = AL_STOPPED;
    seqp->vol = 0x7FFF;
    seqp->debugFlags = c->debugFlags;
    seqp->frameTime = AL_USEC_PER_FRAME;
    seqp->curTime = 0;
    seqp->initOsc = c->initOsc;
    seqp->updateOsc = c->updateOsc;
    seqp->stopOsc = c->stopOsc;
    seqp->unk7C = 0;
    seqp->unk80 = 0;
    seqp->unk84 = 0;

    seqp->nextEvent.type = AL_SEQP_API_EVT;

    seqp->maxChannels = c->maxChannels;
    seqp->chanState = alHeapAlloc(hp, c->maxChannels, 0x10);
    func_002928F0(seqp);

    voices = alHeapAlloc(hp, c->maxVoices, sizeof(ALVoiceState));
    seqp->vFreeList = 0;
    for (i = 0; i < c->maxVoices; i++) {
        vs = &voices[i];
        vs->next = seqp->vFreeList;
        seqp->vFreeList = vs;
    }

    seqp->vAllocHead = 0;
    seqp->vAllocTail = 0;

    items = alHeapAlloc(hp, c->maxEvents, 0x1C);
    func_00290F20(&seqp->evtq, items, c->maxEvents);

    seqp->node.next = 0;
    seqp->node.handler = func_002914B4;
    seqp->node.clientData = seqp;
    func_00293F10(D_800B7760, &seqp->node);
}
