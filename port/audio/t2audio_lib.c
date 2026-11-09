/* t2audio_lib.c: libultra libaudio functions typed for the host (docs/port/AUDIO.md §8).
 *
 * The synthesizer driver, its voice calls and the event-queue helpers as the SDK's
 * synthesizer.c / syn*.c / event.c / sndplayer.c write them, on structs/libaudio.h's types.
 * The game's copies (imported from the Rage Wars decomps) read ALSynth, ALPlayer, ALVoice,
 * PVoice, ALFilter and the event list items through local views at N64 offsets
 * (TimedNode.time at 0x10 = the low half of ALPlayer.handler on the host, Shape_typemap_71's
 * paramSamples at 0x1C, IntegerStateDC_2's offset at 0xD8, ...), so the game's audio frame
 * spun in alAudioFrame (__nextSampleTime never found the client that was due). Those files are
 * out of the link (port/cmake/link.cmake). */
#include "common.h"
#include "structs/libaudio.h"

#include <string.h>

s32 osSetIntMask(s32 mask);
void alLink(ALLink *ln, ALLink *to);
void alUnlink(ALLink *ln);
ALParam *__allocParam(void);

/* ---- synthesizer.c ---- */

/* func_00293F10 */
void alSynAddPlayer(ALSynth *drvr, ALPlayer *client) {
    s32 mask = osSetIntMask(1);
    client->samplesLeft = drvr->curSamples;
    client->next = drvr->head;
    drvr->head = client;
    osSetIntMask(mask);
}

/* func_00294C70 */
s32 __nextSampleTime(ALSynth *drvr, ALPlayer **client) {
    ALMicroTime delta = 0x7FFFFFFF;
    ALPlayer *cl;
    *client = NULL;
    for (cl = drvr->head; cl != NULL; cl = cl->next) {
        if ((cl->samplesLeft - drvr->curSamples) < delta) {
            *client = cl;
            delta = cl->samplesLeft - drvr->curSamples;
        }
    }
    return (*client)->samplesLeft;
}

/* func_00294D18 / func_00294C30 */
s32 _timeToSamples(ALSynth *drvr, s32 micros) {
    return (s32)(((f32)micros * (f32)drvr->outputRate * 0.000001f) + 0.5f);
}
s32 _timeToSamplesNoRound(ALSynth *drvr, s32 micros) {
    return (s32)(((f32)micros * (f32)drvr->outputRate * 0.000001f) + 0.5f) & ~0xF;
}

/* func_00294BDC */
void _collectPVoices(ALSynth *drvr) {
    ALLink *dl;
    while ((dl = drvr->pLameList.next) != NULL) {
        alUnlink(dl);
        alLink(dl, &drvr->pFreeList);
    }
}

/* func_00294B9C (linked to arg0 + 0x14: ALSynth.pLameList; _collectPVoices frees them) */
void _freePVoice(ALSynth *drvr, PVoice *pvoice) {
    alUnlink(&pvoice->node);
    alLink(&pvoice->node, &drvr->pLameList);
}

/* func_00294090 (Node_func_802B3130_de: vvoice->priority, offset at 0xD8) */
s32 _allocatePVoice(ALSynth *drvr, PVoice **pvoice, s16 priority) {
    ALLink *dl;
    s32 stolen = 0;
    if ((dl = drvr->pLameList.next) != NULL || (dl = drvr->pFreeList.next) != NULL) {
        *pvoice = (PVoice *)dl;
        alUnlink(dl);
        alLink(dl, &drvr->pAllocList);
    } else {
        for (dl = drvr->pAllocList.next; dl != NULL; dl = dl->next) {
            PVoice *pv = (PVoice *)dl;
            if (priority >= pv->vvoice->priority && pv->offset == 0) {
                *pvoice = pv;
                priority = pv->vvoice->priority;
                stolen = 1;
            }
        }
    }
    return stolen;
}

/* the common tail of the voice calls: an update on the voice's channel knob */
static ALParam *voice_param(ALSynth *drvr, ALVoice *v, s16 type) {
    ALParam *u = __allocParam();
    if (u != NULL) {
        u->delta = drvr->paramSamples + v->pvoice->offset;
        u->type = type;
        u->next = NULL;
    }
    return u;
}
static void voice_send(ALVoice *v, void *u) {
    ALFilter *f = v->pvoice->channelKnob;
    (*f->setParam)(f, AL_FILTER_ADD_UPDATE, u);
}

/* func_00294160 (the update carries the pvoice) */
void alSynFreeVoice(ALSynth *drvr, ALVoice *voice) {
    if (voice->pvoice != NULL) {
        if (voice->pvoice->offset != 0) {
            ALFreeParam *u = (ALFreeParam *)voice_param(drvr, voice, AL_FILTER_FREE_VOICE);
            if (u != NULL) {
                u->pvoice = voice->pvoice;
                voice_send(voice, u);
                voice->pvoice = NULL;
            }
        } else {
            _freePVoice(drvr, voice->pvoice);
            voice->pvoice = NULL;
        }
    }
}
/* func_00294200 */
void alSynSetFXMix(ALSynth *drvr, ALVoice *v, u8 fxmix) {
    if (v->pvoice != NULL) {
        ALParam *u = voice_param(drvr, v, AL_FILTER_SET_FXAMT);
        if (u != NULL) {
            s8 m = (s8)fxmix;
            u->data.i = (m < 0) ? -m : (u8)m;
            voice_send(v, u);
        }
    }
}
/* func_002942B0 */
void alSynSetPan(ALSynth *drvr, ALVoice *v, u8 pan) {
    if (v->pvoice != NULL) {
        ALParam *u = voice_param(drvr, v, AL_FILTER_SET_PAN);
        if (u != NULL) {
            u->data.i = pan;
            voice_send(v, u);
        }
    }
}
/* func_00294340 */
void alSynSetPitch(ALSynth *drvr, ALVoice *v, f32 pitch) {
    if (v->pvoice != NULL) {
        ALParam *u = voice_param(drvr, v, AL_FILTER_SET_PITCH);
        if (u != NULL) {
            u->data.f = pitch;
            voice_send(v, u);
        }
    }
}
/* func_002943D0 */
void alSynSetPriority(ALSynth *drvr, ALVoice *v, s16 priority) {
    (void)drvr;
    v->priority = priority;
}
/* func_002943E0 */
void alSynSetVol(ALSynth *drvr, ALVoice *v, s16 volume, ALMicroTime t) {
    if (v->pvoice != NULL) {
        ALParam *u = voice_param(drvr, v, AL_FILTER_SET_VOLUME);
        if (u != NULL) {
            u->data.i = volume;
            u->moredata.i = _timeToSamplesNoRound(drvr, t);
            voice_send(v, u);
        }
    }
}
/* func_002944A0 */
void alSynStartVoice(ALSynth *drvr, ALVoice *v, ALWaveTable *table) {
    if (v->pvoice != NULL) {
        ALStartParam *u = (ALStartParam *)voice_param(drvr, v, AL_FILTER_START_VOICE);
        if (u != NULL) {
            u->wave = table;
            u->unity = v->unityPitch;
            voice_send(v, u);
        }
    }
}
/* func_00294650 */
void alSynStopVoice(ALSynth *drvr, ALVoice *v) {
    if (v->pvoice != NULL) {
        ALParam *u = voice_param(drvr, v, AL_FILTER_STOP_VOICE);
        if (u != NULL) {
            voice_send(v, u);
        }
    }
}

/* ---- event.c ---- */

/* func_00291140 */
void alEvtqFlush(ALEventQueue *evtq) {
    ALLink *thisNode;
    ALLink *nextNode;
    s32 mask = osSetIntMask(1);
    for (thisNode = evtq->allocList.next; thisNode != NULL; thisNode = nextNode) {
        nextNode = thisNode->next;
        alUnlink(thisNode);
        alLink(thisNode, &evtq->freeList);
    }
    osSetIntMask(mask);
}
/* func_002911B8 */
void alEvtqFlushType(ALEventQueue *evtq, s16 type) {
    ALLink *thisNode;
    ALLink *nextNode;
    s32 mask = osSetIntMask(1);
    for (thisNode = evtq->allocList.next; thisNode != NULL; thisNode = nextNode) {
        ALEventListItem *thisItem = (ALEventListItem *)thisNode;
        nextNode = thisNode->next;
        if (thisItem->evt.type == type) {
            if (nextNode != NULL) {
                ((ALEventListItem *)nextNode)->delta += thisItem->delta;
            }
            alUnlink(thisNode);
            alLink(thisNode, &evtq->freeList);
        }
    }
    osSetIntMask(mask);
}

/* ---- sndplayer.c ---- */

/* func_00293BA8 (the event's state at item + 0x10) */
void _removeEvents(ALEventQueue *evtq, ALSoundState *state) {
    ALLink *thisNode;
    ALLink *nextNode;
    s32 mask = osSetIntMask(1);
    for (thisNode = evtq->allocList.next; thisNode != NULL; thisNode = nextNode) {
        ALEventListItem *thisItem = (ALEventListItem *)thisNode;
        nextNode = thisNode->next;
        if (((ALSndpEvent *)&thisItem->evt)->common.state == state) {
            if (nextNode != NULL) {
                ((ALEventListItem *)nextNode)->delta += thisItem->delta;
            }
            alUnlink(thisNode);
            alLink(thisNode, &evtq->freeList);
        }
    }
    osSetIntMask(mask);
}

/* ---- csplayer.c ---- */

/* func_0028FA00 (state at 0x2C) */
s32 alCSPGetState(ALCSPlayer *seqp) {
    return seqp->state;
}

/* ---- filter.c / save.c: the filters' constructors ---- */

/* the libaudio.h constants the game's copies use as numbers */
#define AL_SAVE 3
#define AL_SEQP_MIDI_EVT 2
#define AL_SEQP_VOL_EVT 0xA
#define AL_SEQP_SEQ_EVT 0xD
#define AL_SEQP_BANK_EVT 0xE
#define AL_SEQP_PLAY_EVT 0xF
#define AL_SEQP_STOP_EVT 0x11
#define AL_MIDI_ControlChange 0xB0
#define AL_MIDI_FX1_CTRL 0x5B

s32 alSaveParam(void *filter, s32 paramID, void *param);

/* func_00296340 (Request view: the handler and setParam stored as s32) */
void alFilterNew(ALFilter *f, ALCmdHandler h, ALSetParam s, s32 type) {
    f->source = NULL;
    f->handler = h;
    f->setParam = s;
    f->inp = 0;
    f->outp = 0;
    f->type = type;
}

/* func_002954F8 (SaveFilter view: dramout/first at 0x14/0x18, inside the host ALFilter) */
Acmd *alSavePull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
void alSaveNew(ALSave *f) {
    alFilterNew(&f->filter, alSavePull, alSaveParam, AL_SAVE);
    f->dramout = 0;
    f->first = 1;
}

/* func_002973E0 (Unk002973E0 view: the source at 0, dramout at 0x14) */
s32 alSaveParam(void *filter, s32 paramID, void *param) {
    ALSave *a = (ALSave *)filter;
    switch (paramID) {
    case AL_FILTER_SET_SOURCE:
        a->filter.source = (ALFilter *)param;
        break;
    case AL_FILTER_SET_DRAM:
        a->dramout = (sptr)param;
        break;
    }
    return 0;
}

/* ---- csplayer.c: the API calls post events (the queue at 0x48 of the N64 player) ---- */

static void csp_post(ALCSPlayer *seqp, ALEvent *evt) {
    void alEvtqPostEvent(ALEventQueue *evtq, ALEvent *evt, ALMicroTime delta);
    alEvtqPostEvent(&seqp->evtq, evt, 0);
}
/* func_00290E90 */
void alCSPSetSeq(ALCSPlayer *seqp, ALCSeq *seq) {
    ALEvent evt;
    evt.type = AL_SEQP_SEQ_EVT;
    evt.msg.spseq.seq = (void *)seq; /* the ALCSeq, in the ALSeq slot */
    csp_post(seqp, &evt);
}
/* func_00290E10 */
void alCSPSetBank(ALCSPlayer *seqp, ALBank *b) {
    ALEvent evt;
    evt.type = AL_SEQP_BANK_EVT;
    evt.msg.spbank.bank = b;
    csp_post(seqp, &evt);
}
/* func_00290EC0 */
void alCSPSetVol(ALCSPlayer *seqp, s16 vol) {
    ALEvent evt;
    evt.type = AL_SEQP_VOL_EVT;
    evt.msg.spvol.vol = vol;
    csp_post(seqp, &evt);
}
/* func_00290E40 */
void alCSPSetChlFXMix(ALCSPlayer *seqp, u8 chan, u8 fxmix) {
    ALEvent evt;
    evt.type = AL_SEQP_MIDI_EVT;
    evt.msg.midi.ticks = 0;
    evt.msg.midi.status = AL_MIDI_ControlChange | chan;
    evt.msg.midi.byte1 = AL_MIDI_FX1_CTRL;
    evt.msg.midi.byte2 = fxmix;
    csp_post(seqp, &evt);
}
/* func_00290DE0 / func_00290EF0 */
void alCSPPlay(ALCSPlayer *seqp) {
    ALEvent evt;
    evt.type = AL_SEQP_PLAY_EVT;
    csp_post(seqp, &evt);
}
void alCSPStop(ALCSPlayer *seqp) {
    ALEvent evt;
    evt.type = AL_SEQP_STOP_EVT;
    csp_post(seqp, &evt);
}

/* ---- resample.c ---- */

/* func_00296690 (IntegerState34 view: delta/first/motion/upitch/ratio at N64 offsets; the source
 * written as an s32 at 0 and its setParam read at the N64 offset) */
s32 alResampleParam(void *filter, s32 paramID, void *param) {
    ALFilter *f = (ALFilter *)filter;
    ALResampler *r = (ALResampler *)filter;
    union {
        f32 f;
        s32 i;
    } data;
    switch (paramID) {
    case AL_FILTER_SET_SOURCE:
        f->source = (ALFilter *)param;
        break;
    case AL_FILTER_RESET:
        r->delta = 0;
        r->first = 1;
        r->motion = 0; /* AL_STOPPED */
        r->upitch = 0;
        if (f->source != NULL) {
            (*f->source->setParam)(f->source, AL_FILTER_RESET, NULL);
        }
        break;
    case AL_FILTER_START:
        r->motion = 1; /* AL_PLAYING */
        if (f->source != NULL) {
            (*f->source->setParam)(f->source, AL_FILTER_START, NULL);
        }
        break;
    case AL_FILTER_SET_PITCH:
        data.i = (s32)(sptr)param; /* the pitch's float bits, passed as the pointer argument */
        r->ratio = data.f;
        break;
    case AL_FILTER_SET_UNITY_PITCH:
        r->upitch = 1;
        break;
    default:
        if (f->source != NULL) {
            (*f->source->setParam)(f->source, paramID, param);
        }
        break;
    }
    return 0;
}

/* ---- the game's ADPCM codebook cache (AudioDMA_CacheBlock, func_0026FFA0) ----
 * alAdpcmPull copies each wave's codebook into a cached block it hands the microcode. The
 * game's entries (CacheEntry: next, key, state, size, 0x80 data bytes) are allocated as 0x90
 * bytes, too small with host pointers, and the list head is a 4-byte game global; here they are
 * host structs on a host list. __amMain's ageing loop (every 21 retraces, each entry's state
 * counts down; an entry at 0 is reused) is T2Audio_DmaCacheAge, called by t2audio_mgr.c. */
typedef struct T2DmaCacheEntry {
    struct T2DmaCacheEntry *next;
    void *key;
    s32 state;
    s32 size;
    u8 data[0x80];
} T2DmaCacheEntry;
static T2DmaCacheEntry *sDmaCache;
extern ALHeap *gAudioHeap;
extern s32 sAudioDMACacheBlocks;
void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
u32 osVirtualToPhysical(void *p);

u32 AudioDMA_CacheBlock(void *block, s32 size) {
    T2DmaCacheEntry *e;
    T2DmaCacheEntry *unused = NULL;
    if (size > (s32)sizeof(e->data)) {
        size = sizeof(e->data);
    }
    for (e = sDmaCache; e != NULL; e = e->next) {
        if (e->key == block) {
            e->state = 3;
            if (e->size != size) {
                e->size = size;
                memcpy(e->data, block, size);
            }
            return osVirtualToPhysical(e->data);
        }
        if (e->state == 0) {
            unused = e;
        }
    }
    if (unused == NULL) {
        unused = alHeapDBAlloc(0, 0, gAudioHeap, 1, sizeof(T2DmaCacheEntry));
        unused->next = sDmaCache;
        sDmaCache = unused;
        sAudioDMACacheBlocks++;
    }
    unused->key = block;
    unused->state = 3;
    unused->size = size;
    memcpy(unused->data, block, size);
    return osVirtualToPhysical(unused->data);
}

void T2Audio_DmaCacheAge(void) {
    T2DmaCacheEntry *e;
    for (e = sDmaCache; e != NULL; e = e->next) {
        if (e->state != 0) {
            e->state--;
        }
    }
}
