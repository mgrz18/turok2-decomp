/* t2audio_csp.c: libultra's compressed sequence player, typed for the host
 * (docs/port/AUDIO.md §9).
 *
 * The SDK's csplayer.c (alCSPNew, __CSPPostNextSeqEvent, __CSPHandleMetaMsg, __CSPRepostEvent,
 * __setUsptFromTempo), cseq.c (alCSeq*: the .cmf track reader) and the sequence-player helpers
 * from seqplayer.c that the compressed player shares (__mapVoice, __unmapVoice, __lookupVoice,
 * __lookupSoundQuick, __vsVol/__vsDelta/__vsPan, __seqpReleaseVoice, __voiceNeedsNoteKill,
 * __initFromBank, __setInstChanState, __resetPerfChanState, __initChanState, __seqpStopOsc), on
 * structs/libaudio.h's ALCSPlayer / ALCSeq / ALVoiceState. Turok 2's copies are the Rage Wars
 * imports: local views at N64 offsets (AnimState.frames at 0x24 for ALCSPlayer.uspt, the event
 * list as Node_func_802AFD6C_de, RuntimeState for ALCSeq, ...) and pointers passed as s32
 * (alCSeqNew's base, __alCSeqNextDelta's sequence, alCSPNew's heap blocks). Their behaviour is
 * the SDK's (checked file by file: the same constants, loop and backup codes, the binary
 * search in __lookupSoundQuick, the same rounding in __vsVol), so these follow the SDK source.
 * The originals are out of the link (port/cmake/link.cmake). __CSPVoiceHandler,
 * __CSPHandleNextSeqEvent and __CSPHandleMIDIMsg stay the game's (already typed).
 *
 * Return types are widened to s32 where the game's prototypes disagree (__vsVol s16/s32,
 * __vsPan ALPan/s32, __alCSeqNextDelta char/s32, __voiceNeedsNoteKill u8/s32): the value is
 * the SDK's, extended, so every caller reads the same thing. */
#include "common.h"
#include "structs/libaudio.h"
#include "globals/libaudio.h"

s32 osSetIntMask(s32 mask);
void alLink(ALLink *ln, ALLink *to);
void alUnlink(ALLink *ln);
void alEvtqNew(ALEventQueue *evtq, ALEventListItem *items, s32 itemCount);
void alEvtqPostEvent(ALEventQueue *evtq, ALEvent *evt, ALMicroTime delta);
void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
void alSynAddPlayer(ALSynth *drvr, ALPlayer *client);
void alSynSetPriority(ALSynth *drvr, ALVoice *voice, s16 priority);
void alSynSetVol(ALSynth *drvr, ALVoice *voice, s16 vol, ALMicroTime delta);
ALMicroTime __CSPVoiceHandler(void *node);

#define AL_SEQ_REF_EVT 0
#define AL_TEMPO_EVT 3
#define AL_SEQ_END_EVT 4
#define AL_NOTE_END_EVT 5
#define AL_SEQP_ENV_EVT 6
#define AL_SEQP_API_EVT 9
#define AL_TRACK_END 18
#define AL_CSP_LOOPSTART 19
#define AL_CSP_LOOPEND 20
#define AL_CSP_NOTEOFF_EVT 21
#define AL_TREM_OSC_EVT 22
#define AL_VIB_OSC_EVT 23

#define AL_STOPPED 0
#define AL_PLAYING 1

#define AL_PHASE_ATTACK 0
#define AL_PHASE_NOTEON 0
#define AL_PHASE_SUSTAIN 2
#define AL_PHASE_RELEASE 3
#define AL_PHASE_SUSTREL 4

#define AL_MIDI_Meta 0xFF
#define AL_MIDI_META_TEMPO 0x51
#define AL_MIDI_META_EOT 0x2F
#define AL_CMIDI_LOOPSTART_CODE 0x2E
#define AL_CMIDI_LOOPEND_CODE 0x2D
#define AL_CMIDI_BLOCK_CODE 0xFE
#define AL_MIDI_NoteOn 0x90
#define AL_MIDI_ProgramChange 0xC0
#define AL_MIDI_ChannelPressure 0xD0

#define AL_USEC_PER_FRAME 16000
#define AL_GAIN_CHANGE_TIME 1000
#define AL_PAN_CENTER 64
#define AL_PAN_LEFT 0
#define AL_PAN_RIGHT 127
#define AL_VOL_FULL 127
#define AL_DEFAULT_PRIORITY 5
#define AL_DEFAULT_FXMIX 0
#define AL_FX_NONE 0

/* ---------------------------------------------------------------- csplayer.c */

void __initChanState(ALSeqPlayer *seqp);

/* func_0028FA10 */
void alCSPNew(ALCSPlayer *seqp, ALSeqpConfig *c) {
    s32 i;
    ALEventListItem *items;
    ALVoiceState *voices;
    ALHeap *hp = c->heap;

    seqp->bank = NULL;
    seqp->target = NULL;
    seqp->drvr = &alGlobals->drvr;
    seqp->chanMask = 0xFF;
    seqp->uspt = 488;
    seqp->nextDelta = 0;
    seqp->state = AL_STOPPED;
    seqp->vol = 0x7FFF;
    seqp->frameTime = AL_USEC_PER_FRAME;
    seqp->curTime = 0;
    seqp->initOsc = (ALOscInit)c->initOsc;
    seqp->updateOsc = (ALOscUpdate)c->updateOsc;
    seqp->stopOsc = (ALOscStop)c->stopOsc;
    seqp->nextEvent.type = AL_SEQP_API_EVT;

    seqp->maxChannels = c->maxChannels;
    seqp->chanState = alHeapDBAlloc(NULL, 0, hp, c->maxChannels, sizeof(ALChanState));
    __initChanState((ALSeqPlayer *)seqp);

    voices = alHeapDBAlloc(NULL, 0, hp, c->maxVoices, sizeof(ALVoiceState));
    seqp->vFreeList = NULL;
    for (i = 0; i < c->maxVoices; i++) {
        voices[i].next = seqp->vFreeList;
        seqp->vFreeList = &voices[i];
    }
    seqp->vAllocHead = NULL;
    seqp->vAllocTail = NULL;

    items = alHeapDBAlloc(NULL, 0, hp, c->maxEvents, sizeof(ALEventListItem));
    alEvtqNew(&seqp->evtq, items, c->maxEvents);

    seqp->node.next = NULL;
    seqp->node.handler = __CSPVoiceHandler;
    seqp->node.clientData = seqp;
    alSynAddPlayer(&alGlobals->drvr, &seqp->node);
}

s32 __alCSeqNextDelta(ALCSeq *seq, s32 *pDeltaTicks);

/* func_00290C60 */
void __CSPPostNextSeqEvent(ALCSPlayer *seqp) {
    ALEvent evt;
    s32 deltaTicks;
    if (seqp->state != AL_PLAYING || seqp->target == NULL)
        return;
    if (!__alCSeqNextDelta(seqp->target, &deltaTicks))
        return;
    evt.type = AL_SEQ_REF_EVT;
    alEvtqPostEvent(&seqp->evtq, &evt, deltaTicks * seqp->uspt);
}

/* func_00290D5C: csplayer.c's static __setUsptFromTempo */
void __CSPSetUsptFromTempo(ALCSPlayer *seqp, f32 tempo) {
    if (seqp->target != NULL)
        seqp->uspt = (s32)((f32)tempo * seqp->target->qnpt);
    else
        seqp->uspt = 488;
}

/* func_00290CCC */
void __CSPRepostEvent(ALEventQueue *evtq, ALEventListItem *item) {
    s32 mask = osSetIntMask(1);
    ALLink *node;
    for (node = &evtq->allocList; node != NULL; node = node->next) {
        if (node->next == NULL) {
            alLink(&item->node, node);
            break;
        } else {
            ALEventListItem *nextItem = (ALEventListItem *)node->next;
            if (item->delta < nextItem->delta) {
                nextItem->delta -= item->delta;
                alLink(&item->node, node);
                break;
            }
            item->delta -= nextItem->delta;
        }
    }
    osSetIntMask(mask);
}

/* func_00290ACC */
void __CSPHandleMetaMsg(ALCSPlayer *seqp, ALEvent *event) {
    ALTempoEvent *tevt = &event->msg.tempo;
    s32 tempo, oldUspt;
    u32 ticks;
    ALMicroTime tempDelta, curDelta = 0;
    ALEventListItem *thisNode, *nextNode, *firstTemp = NULL;

    if (tevt->status != AL_MIDI_Meta || tevt->type != AL_MIDI_META_TEMPO)
        return;
    oldUspt = seqp->uspt;
    tempo = (tevt->byte1 << 16) | (tevt->byte2 << 8) | (tevt->byte3 << 0);
    __CSPSetUsptFromTempo(seqp, (f32)tempo);

    /* re-time the queued note offs (they were posted in microseconds at the old tempo) */
    thisNode = (ALEventListItem *)seqp->evtq.allocList.next;
    while (thisNode != NULL) {
        curDelta += thisNode->delta;
        nextNode = (ALEventListItem *)thisNode->node.next;
        if (thisNode->evt.type == AL_CSP_NOTEOFF_EVT) {
            alUnlink(&thisNode->node);
            if (firstTemp != NULL) {
                alLink(&thisNode->node, &firstTemp->node);
            } else {
                thisNode->node.next = NULL;
                thisNode->node.prev = NULL;
                firstTemp = thisNode;
            }
            tempDelta = curDelta;
            if (nextNode != NULL) {
                curDelta -= thisNode->delta;
                nextNode->delta += thisNode->delta;
            }
            thisNode->delta = tempDelta;
        }
        thisNode = nextNode;
    }
    thisNode = firstTemp;
    while (thisNode != NULL) {
        nextNode = (ALEventListItem *)thisNode->node.next;
        ticks = thisNode->delta / oldUspt;
        thisNode->delta = ticks * seqp->uspt;
        __CSPRepostEvent(&seqp->evtq, thisNode);
        thisNode = nextNode;
    }
}

/* ---------------------------------------------------------------- cseq.c */

/* func_0028F460 */
u8 __getTrackByte(ALCSeq *seq, u32 track) {
    u8 theByte;
    if (seq->curBULen[track]) {
        theByte = *seq->curBUPtr[track];
        seq->curBUPtr[track]++;
        seq->curBULen[track]--;
    } else {
        theByte = *seq->curLoc[track];
        seq->curLoc[track]++;
        if (theByte == AL_CMIDI_BLOCK_CODE) {
            u8 loBackUp, hiBackUp, theLen, nextByte;
            u32 backup;
            nextByte = *seq->curLoc[track];
            seq->curLoc[track]++;
            if (nextByte != AL_CMIDI_BLOCK_CODE) {
                /* a backup section: repeat theLen bytes from (backup + 4) bytes back */
                hiBackUp = nextByte;
                loBackUp = *seq->curLoc[track];
                seq->curLoc[track]++;
                theLen = *seq->curLoc[track];
                seq->curLoc[track]++;
                backup = ((u32)hiBackUp << 8) + loBackUp;
                seq->curBUPtr[track] = seq->curLoc[track] - (backup + 4);
                seq->curBULen[track] = theLen;
                theByte = *seq->curBUPtr[track];
                seq->curBUPtr[track]++;
                seq->curBULen[track]--;
            }
        }
    }
    return theByte;
}

/* func_0028F98C */
u32 __readVarLen(ALCSeq *seq, u32 track) {
    u32 value = __getTrackByte(seq, track), c;
    if (value & 0x80) {
        value &= 0x7F;
        do {
            c = __getTrackByte(seq, track);
            value = (value << 7) + (c & 0x7F);
        } while (c & 0x80);
    }
    return value;
}

/* func_0028F518. The header's track offsets and division are host order (T2Audio_SwapSoundData
 * swaps ALCMidiHdr). */
void alCSeqNew(ALCSeq *seq, u8 *ptr) {
    u32 i, tmpOff;
    seq->base = (ALCMidiHdr *)ptr;
    seq->validTracks = 0;
    seq->lastDeltaTicks = 0;
    seq->lastTicks = 0;
    seq->deltaFlag = 1;
    for (i = 0; i < 16; i++) {
        seq->lastStatus[i] = 0;
        seq->curBUPtr[i] = NULL;
        seq->curBULen[i] = 0;
        tmpOff = seq->base->trackOffset[i];
        if (tmpOff != 0) {
            seq->validTracks |= 1u << i;
            seq->curLoc[i] = ptr + tmpOff;
            seq->evtDeltaTicks[i] = __readVarLen(seq, i);
        } else {
            seq->curLoc[i] = NULL;
        }
    }
    seq->qnpt = 1.0f / (f32)seq->base->division;
}

/* func_0028EF90 */
s32 __alCSeqGetTrackEvent(ALCSeq *seq, u32 track, ALEvent *event) {
    u32 offset;
    u8 status, loopCt, curLpCt, *tmpPtr;

    status = __getTrackByte(seq, track);
    if (status == AL_MIDI_Meta) {
        u8 type = __getTrackByte(seq, track);
        if (type == AL_MIDI_META_TEMPO) {
            event->type = AL_TEMPO_EVT;
            event->msg.tempo.status = status;
            event->msg.tempo.type = type;
            event->msg.tempo.byte1 = __getTrackByte(seq, track);
            event->msg.tempo.byte2 = __getTrackByte(seq, track);
            event->msg.tempo.byte3 = __getTrackByte(seq, track);
            seq->lastStatus[track] = 0;
        } else if (type == AL_MIDI_META_EOT) {
            seq->validTracks ^= 1u << track;
            event->type = seq->validTracks ? AL_TRACK_END : AL_SEQ_END_EVT;
        } else if (type == AL_CMIDI_LOOPSTART_CODE) {
            __getTrackByte(seq, track);
            __getTrackByte(seq, track);
            seq->lastStatus[track] = 0;
            event->type = AL_CSP_LOOPSTART;
        } else if (type == AL_CMIDI_LOOPEND_CODE) {
            tmpPtr = seq->curLoc[track];
            loopCt = *tmpPtr++;
            curLpCt = *tmpPtr;
            if (curLpCt == 0) {
                *tmpPtr = loopCt;
                seq->curLoc[track] = tmpPtr + 5;
            } else {
                if (curLpCt != 0xFF)
                    *tmpPtr = curLpCt - 1;
                tmpPtr++;
                offset = (u32)*tmpPtr++ << 24;
                offset += (u32)*tmpPtr++ << 16;
                offset += (u32)*tmpPtr++ << 8;
                offset += *tmpPtr++;
                seq->curLoc[track] = tmpPtr - offset;
            }
            seq->lastStatus[track] = 0;
            event->type = AL_CSP_LOOPEND;
        }
    } else {
        event->type = 1; /* AL_SEQ_MIDI_EVT */
        if (status & 0x80) {
            event->msg.midi.status = status;
            event->msg.midi.byte1 = __getTrackByte(seq, track);
            seq->lastStatus[track] = status;
        } else {
            event->msg.midi.status = seq->lastStatus[track];
            event->msg.midi.byte1 = status;
        }
        if ((event->msg.midi.status & 0xF0) != AL_MIDI_ProgramChange &&
            (event->msg.midi.status & 0xF0) != AL_MIDI_ChannelPressure) {
            event->msg.midi.byte2 = __getTrackByte(seq, track);
            if ((event->msg.midi.status & 0xF0) == AL_MIDI_NoteOn)
                event->msg.midi.duration = __readVarLen(seq, track);
        } else {
            event->msg.midi.byte2 = 0;
        }
    }
    return 1;
}

/* func_0028F61C */
void alCSeqNextEvent(ALCSeq *seq, ALEvent *evt) {
    u32 i, firstTime = 0xFFFFFFFF, firstTrack = 0, lastTicks = seq->lastDeltaTicks;
    for (i = 0; i < 16; i++) {
        if ((seq->validTracks >> i) & 1) {
            if (seq->deltaFlag)
                seq->evtDeltaTicks[i] -= lastTicks;
            if (seq->evtDeltaTicks[i] < firstTime) {
                firstTime = seq->evtDeltaTicks[i];
                firstTrack = i;
            }
        }
    }
    __alCSeqGetTrackEvent(seq, firstTrack, evt);
    evt->msg.midi.ticks = firstTime;
    seq->lastTicks += firstTime;
    seq->lastDeltaTicks = firstTime;
    if (evt->type != AL_TRACK_END)
        seq->evtDeltaTicks[firstTrack] += __readVarLen(seq, firstTrack);
    seq->deltaFlag = 1;
}

/* func_0028F910: FALSE when no track has data left */
s32 __alCSeqNextDelta(ALCSeq *seq, s32 *pDeltaTicks) {
    u32 i, firstTime = 0xFFFFFFFF, lastTicks = seq->lastDeltaTicks;
    if (!seq->validTracks)
        return 0;
    for (i = 0; i < 16; i++) {
        if ((seq->validTracks >> i) & 1) {
            if (seq->deltaFlag)
                seq->evtDeltaTicks[i] -= lastTicks;
            if (seq->evtDeltaTicks[i] < firstTime)
                firstTime = seq->evtDeltaTicks[i];
        }
    }
    seq->deltaFlag = 0;
    *pDeltaTicks = (s32)firstTime;
    return 1;
}

/* func_0028F71C */
f32 alCSeqTicksToSec(ALCSeq *seq, s32 ticks, u32 tempo) {
    return (f32)(((f32)ticks * (f32)tempo) / ((f32)seq->base->division * 1000000.0));
}

/* func_0028F784 */
u32 alCSeqSecToTicks(ALCSeq *seq, f32 sec, u32 tempo) {
    return (u32)(((sec * 1000000.0) * seq->base->division) / tempo);
}

/* func_0028F710 */
s32 alCSeqGetTicks(ALCSeq *seq) {
    return (s32)seq->lastTicks;
}

/* func_0028F1F0 */
void alCSeqNewMarker(ALCSeq *seq, ALCSeqMarker *m, u32 ticks) {
    ALEvent evt;
    ALCSeq tempSeq;
    s32 i;
    alCSeqNew(&tempSeq, (u8 *)seq->base);
    do {
        m->validTracks = tempSeq.validTracks;
        m->lastTicks = tempSeq.lastTicks;
        m->lastDeltaTicks = tempSeq.lastDeltaTicks;
        for (i = 0; i < 16; i++) {
            m->curLoc[i] = tempSeq.curLoc[i];
            m->curBUPtr[i] = tempSeq.curBUPtr[i];
            m->curBULen[i] = tempSeq.curBULen[i];
            m->lastStatus[i] = tempSeq.lastStatus[i];
            m->evtDeltaTicks[i] = tempSeq.evtDeltaTicks[i];
        }
        alCSeqNextEvent(&tempSeq, &evt);
        if (evt.type == AL_SEQ_END_EVT)
            break;
    } while (tempSeq.lastTicks < ticks);
}

/* func_0028F828 */
void alCSeqSetLoc(ALCSeq *seq, ALCSeqMarker *m) {
    s32 i;
    seq->validTracks = m->validTracks;
    seq->lastTicks = m->lastTicks;
    seq->lastDeltaTicks = m->lastDeltaTicks;
    for (i = 0; i < 16; i++) {
        seq->curLoc[i] = m->curLoc[i];
        seq->curBUPtr[i] = m->curBUPtr[i];
        seq->curBULen[i] = m->curBULen[i];
        seq->lastStatus[i] = m->lastStatus[i];
        seq->evtDeltaTicks[i] = m->evtDeltaTicks[i];
    }
}

/* func_0028F89C */
void alCSeqGetLoc(ALCSeq *seq, ALCSeqMarker *m) {
    s32 i;
    m->validTracks = seq->validTracks;
    m->lastTicks = seq->lastTicks;
    m->lastDeltaTicks = seq->lastDeltaTicks;
    for (i = 0; i < 16; i++) {
        m->curLoc[i] = seq->curLoc[i];
        m->curBUPtr[i] = seq->curBUPtr[i];
        m->curBULen[i] = seq->curBULen[i];
        m->lastStatus[i] = seq->lastStatus[i];
        m->evtDeltaTicks[i] = seq->evtDeltaTicks[i];
    }
}

/* ---------------------------------------------------------------- seqplayer.c helpers */

/* func_0029247C */
ALVoiceState *__mapVoice(ALSeqPlayer *seqp, u8 key, u8 vel, u8 channel) {
    ALVoiceState *vs = seqp->vFreeList;
    if (vs != NULL) {
        seqp->vFreeList = vs->next;
        vs->next = NULL;
        if (seqp->vAllocHead == NULL)
            seqp->vAllocHead = vs;
        else
            seqp->vAllocTail->next = vs;
        seqp->vAllocTail = vs;
        vs->channel = channel;
        vs->key = key;
        vs->velocity = vel;
        vs->voice.clientPrivate = vs;
    }
    return vs;
}

/* func_002924CC */
void __unmapVoice(ALSeqPlayer *seqp, ALVoice *voice) {
    ALVoiceState *prev = NULL, *vs;
    for (vs = seqp->vAllocHead; vs != NULL; vs = vs->next) {
        if (&vs->voice == voice) {
            if (prev != NULL)
                prev->next = vs->next;
            else
                seqp->vAllocHead = vs->next;
            if (vs == seqp->vAllocTail)
                seqp->vAllocTail = prev;
            vs->next = seqp->vFreeList;
            seqp->vFreeList = vs;
            return;
        }
        prev = vs;
    }
}

/* func_002925E0 */
ALVoiceState *__lookupVoice(ALSeqPlayer *seqp, u8 key, u8 channel) {
    ALVoiceState *vs;
    for (vs = seqp->vAllocHead; vs != NULL; vs = vs->next)
        if (vs->key == key && vs->channel == channel && vs->phase != AL_PHASE_RELEASE &&
            vs->phase != AL_PHASE_SUSTREL)
            return vs;
    return NULL;
}

/* func_00292640 (callers pass key/vel/chan as s32: the low bytes count) */
ALSound *__lookupSoundQuick(ALSeqPlayer *seqp, s32 key_, s32 vel_, s32 chan_) {
    u8 key = (u8)key_, vel = (u8)vel_, chan = (u8)chan_;
    ALInstrument *inst = seqp->chanState[chan].instrument;
    s32 l = 1, r, i;
    ALKeyMap *keymap;
    if (inst == NULL)
        return NULL;
    r = inst->soundCount;
    while (r >= l) {
        i = (l + r) / 2;
        keymap = inst->soundArray[i - 1]->keyMap;
        if (key >= keymap->keyMin && key <= keymap->keyMax && vel >= keymap->velocityMin &&
            vel <= keymap->velocityMax)
            return inst->soundArray[i - 1];
        else if (key < keymap->keyMin || (vel < keymap->velocityMin && key <= keymap->keyMax))
            r = i - 1;
        else
            l = i + 1;
    }
    return NULL;
}

/* func_00292774: the voice's target volume (velocity, envelope, tremolo, sample, channel and
 * player volume) */
s32 __vsVol(ALVoiceState *vs, ALSeqPlayer *seqp) {
    u32 t1, t2;
    t1 = (vs->tremelo * vs->velocity * vs->envGain) >> 6;
    t2 = (vs->sound->sampleVolume * seqp->vol * seqp->chanState[vs->channel].vol) >> 14;
    t1 *= t2;
    t1 >>= 15;
    return (s16)t1;
}

/* func_002927E8 */
s32 __vsDelta(ALVoiceState *vs, s32 t) {
    s32 delta = vs->envEndTime - t;
    return delta >= 0 ? delta : AL_GAIN_CHANGE_TIME;
}

/* func_00292804 */
s32 __vsPan(ALVoiceState *vs, ALSeqPlayer *seqp) {
    s32 tmp = seqp->chanState[vs->channel].pan - AL_PAN_CENTER + vs->sound->samplePan;
    if (tmp < AL_PAN_LEFT)
        tmp = AL_PAN_LEFT;
    if (tmp > AL_PAN_RIGHT)
        tmp = AL_PAN_RIGHT;
    return (u8)tmp;
}

/* func_00292A74 */
void __seqpReleaseVoice(ALSeqPlayer *seqp, ALVoice *voice, ALMicroTime deltaTime) {
    ALEvent evt;
    ALVoiceState *vs = (ALVoiceState *)voice->clientPrivate;
    if (vs->envPhase == AL_PHASE_ATTACK) {
        /* drop the pending envelope events of this voice */
        ALLink *thisNode = seqp->evtq.allocList.next, *nextNode;
        while (thisNode != NULL) {
            ALEventListItem *thisItem = (ALEventListItem *)thisNode, *nextItem;
            nextNode = thisNode->next;
            nextItem = (ALEventListItem *)nextNode;
            if (thisItem->evt.type == AL_SEQP_ENV_EVT && thisItem->evt.msg.vol.voice == voice) {
                if (nextItem != NULL)
                    nextItem->delta += thisItem->delta;
                alUnlink(thisNode);
                alLink(thisNode, &seqp->evtq.freeList);
            }
            thisNode = nextNode;
        }
    }
    vs->velocity = 0;
    vs->envPhase = AL_PHASE_RELEASE;
    vs->envGain = 0;
    vs->envEndTime = seqp->curTime + deltaTime;
    alSynSetPriority(seqp->drvr, voice, 0);
    alSynSetVol(seqp->drvr, voice, 0, deltaTime);
    evt.type = AL_NOTE_END_EVT;
    evt.msg.note.voice = voice;
    alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);
}

/* func_00292530 */
s32 __voiceNeedsNoteKill(ALSeqPlayer *seqp, ALVoice *voice, ALMicroTime killTime) {
    ALLink *thisNode = seqp->evtq.allocList.next, *nextNode;
    ALMicroTime itemTime = 0;
    s32 needsNoteKill = 1;
    while (thisNode != NULL) {
        ALEventListItem *thisItem = (ALEventListItem *)thisNode;
        nextNode = thisNode->next;
        itemTime += thisItem->delta;
        if (thisItem->evt.type == AL_NOTE_END_EVT && thisItem->evt.msg.note.voice == voice) {
            if (itemTime > killTime) {
                if (nextNode != NULL)
                    ((ALEventListItem *)nextNode)->delta += thisItem->delta;
                alUnlink(thisNode);
                alLink(thisNode, &seqp->evtq.freeList);
            } else {
                needsNoteKill = 0;
            }
            break;
        }
        thisNode = nextNode;
    }
    return needsNoteKill;
}

/* func_00292A20 */
void __setInstChanState(ALSeqPlayer *seqp, ALInstrument *inst, s32 chan) {
    seqp->chanState[chan].instrument = inst;
    seqp->chanState[chan].pan = inst->pan;
    seqp->chanState[chan].vol = inst->volume;
    seqp->chanState[chan].priority = inst->priority;
    seqp->chanState[chan].bendRange = inst->bendRange;
}

/* func_002929A0 */
void __resetPerfChanState(ALSeqPlayer *seqp, s32 chan) {
    seqp->chanState[chan].fxId = AL_FX_NONE;
    seqp->chanState[chan].fxmix = AL_DEFAULT_FXMIX;
    seqp->chanState[chan].pan = AL_PAN_CENTER;
    seqp->chanState[chan].vol = AL_VOL_FULL;
    seqp->chanState[chan].priority = AL_DEFAULT_PRIORITY;
    seqp->chanState[chan].sustain = 0;
    seqp->chanState[chan].bendRange = 200;
    seqp->chanState[chan].pitchBend = 1.0f;
}

/* func_002928F0 */
void __initChanState(ALSeqPlayer *seqp) {
    s32 i;
    for (i = 0; i < seqp->maxChannels; i++) {
        seqp->chanState[i].instrument = NULL;
        __resetPerfChanState(seqp, i);
    }
}

/* func_00292844 */
void __initFromBank(ALSeqPlayer *seqp, ALBank *b) {
    s32 i;
    ALInstrument *inst = NULL;
    for (i = 0; inst == NULL; i++)
        inst = b->instArray[i];
    for (i = 0; i < seqp->maxChannels; i++) {
        __resetPerfChanState(seqp, i);
        __setInstChanState(seqp, inst, i);
    }
    if (b->percussion != NULL) {
        __resetPerfChanState(seqp, i);
        __setInstChanState(seqp, b->percussion, 9);
    }
}

/* func_00292B94 */
void __seqpStopOsc(ALSeqPlayer *seqp, ALVoiceState *vs) {
    ALEventListItem *thisNode = (ALEventListItem *)seqp->evtq.allocList.next, *nextNode;
    while (thisNode != NULL) {
        s16 evtType = thisNode->evt.type;
        nextNode = (ALEventListItem *)thisNode->node.next;
        if ((evtType == AL_TREM_OSC_EVT || evtType == AL_VIB_OSC_EVT) && thisNode->evt.msg.osc.vs == vs) {
            (*seqp->stopOsc)(thisNode->evt.msg.osc.oscState);
            alUnlink(&thisNode->node);
            if (nextNode != NULL)
                nextNode->delta += thisNode->delta;
            alLink(&thisNode->node, &seqp->evtq.freeList);
            vs->flags &= evtType == AL_TREM_OSC_EVT ? 0xFE : 0xFD;
            if (!vs->flags)
                return;
        }
        thisNode = nextNode;
    }
}
