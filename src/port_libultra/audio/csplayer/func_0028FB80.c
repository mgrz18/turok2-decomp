/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */
/* libultra (2.0) audio/csplayer.c, __CSPVoiceHandler: the compact sequence player callback, with its asserts. */
#include "common.h"
#include "structs/libaudio.h"

#define AL_SEQ_REF_EVT          0
#define AL_SEQ_MIDI_EVT         1
#define AL_SEQP_MIDI_EVT        2
#define AL_TEMPO_EVT            3
#define AL_SEQ_END_EVT          4
#define AL_NOTE_END_EVT         5
#define AL_SEQP_ENV_EVT         6
#define AL_SEQP_META_EVT        7
#define AL_SEQP_API_EVT         9
#define AL_SEQP_VOL_EVT         10
#define AL_SEQP_PRIORITY_EVT    12
#define AL_SEQP_SEQ_EVT         13
#define AL_SEQP_BANK_EVT        14
#define AL_SEQP_PLAY_EVT        15
#define AL_SEQP_STOP_EVT        16
#define AL_SEQP_STOPPING_EVT    17
#define AL_CSP_NOTEOFF_EVT      21
#define AL_TREM_OSC_EVT         22
#define AL_VIB_OSC_EVT          23

#define AL_STOPPED      0
#define AL_PLAYING      1
#define AL_STOPPING     2

#define AL_PHASE_ATTACK 0
#define AL_PHASE_DECAY  1

#define AL_EVTQ_END     0x7FFFFFFF
#define KILL_TIME       50000

extern void __assert(const char *exp, const char *file, s32 line);   /* __assert */
#define assert(EX, line) ((EX) ? (void)0 : __assert("EX", "audio/csplayer.c", line))

extern void __CSPHandleNextSeqEvent(ALCSPlayer *seqp);                                 /* __CSPHandleNextSeqEvent */
extern void alEvtqPostEvent(ALEventQueue *evtq, ALEvent *evt, ALMicroTime delta); /* alEvtqPostEvent */
extern void alSynStopVoice(void *drvr, ALVoice *voice);
extern void alSynFreeVoice(void *drvr, ALVoice *voice);                       /* alSynFreeVoice */
extern void __seqpStopOsc(ALCSPlayer *seqp, ALVoiceState *vs);               /* __seqpStopOsc */
extern void __unmapVoice(ALCSPlayer *seqp, ALVoice *voice);                 /* __unmapVoice */
extern s16 __vsVol(ALVoiceState *vs, ALCSPlayer *seqp);                /* __vsVol */
extern ALMicroTime __vsDelta(ALVoiceState *vs, ALMicroTime t);           /* __vsDelta */
extern void alSynSetVol(void *drvr, ALVoice *voice, s16 vol, ALMicroTime delta);
extern void alSynSetPitch(void *drvr, ALVoice *voice, f32 pitch);            /* alSynSetPitch */
extern void __CSPHandleMIDIMsg(ALCSPlayer *seqp, ALEvent *event);                 /* __CSPHandleMIDIMsg */
extern void __CSPHandleMetaMsg(ALCSPlayer *seqp, ALEvent *event);
extern void __CSPPostNextSeqEvent(ALCSPlayer *seqp);                                 /* __CSPPostNextSeqEvent */
extern void alEvtqFlushType(ALEventQueue *evtq, s16 type);                     /* alEvtqFlushType */
extern u8 __voiceNeedsNoteKill(ALCSPlayer *seqp, ALVoice *voice, ALMicroTime killTime); /* __voiceNeedsNoteKill */
extern void __seqpReleaseVoice(ALCSPlayer *seqp, ALVoice *voice, ALMicroTime deltaTime); /* __seqpReleaseVoice */
extern void __CSPSetUsptFromTempo(ALCSPlayer *seqp, f32 tempo);                      /* __setUsptFromTempo */
extern void __initFromBank(ALCSPlayer *seqp, void *bank);                     /* __initFromBank */
extern ALMicroTime alEvtqNextEvent(ALEventQueue *evtq, ALEvent *evt);          /* alEvtqNextEvent */

/* __CSPVoiceHandler */
ALMicroTime __CSPVoiceHandler(void *node) {
    ALCSPlayer *seqp = (ALCSPlayer *)node;
    ALEvent evt;
    ALVoice *voice;
    ALMicroTime delta;
    ALVoiceState *vs;
    void *oscState;
    f32 oscValue;
    u8 chan;

    do {
        switch (seqp->nextEvent.type) {
        case AL_SEQ_REF_EVT:
            __CSPHandleNextSeqEvent(seqp);
            break;

        case AL_SEQP_API_EVT:
            evt.type = AL_SEQP_API_EVT;
            alEvtqPostEvent(&seqp->evtq, &evt, seqp->frameTime);
            break;

        case AL_NOTE_END_EVT:
            voice = seqp->nextEvent.msg.note.voice;
            alSynStopVoice(seqp->drvr, voice);
            alSynFreeVoice(seqp->drvr, voice);
            vs = (ALVoiceState *)voice->clientPrivate;
            if (vs->flags) {
                __seqpStopOsc(seqp, vs);
            }
            __unmapVoice(seqp, voice);
            break;

        case AL_SEQP_ENV_EVT:
            voice = seqp->nextEvent.msg.vol.voice;
            vs = (ALVoiceState *)voice->clientPrivate;
            if (vs->envPhase == AL_PHASE_ATTACK) {
                vs->envPhase = AL_PHASE_DECAY;
            }
            delta = seqp->nextEvent.msg.vol.delta;
            vs->envEndTime = seqp->curTime + delta;
            vs->envGain = seqp->nextEvent.msg.vol.vol;
            alSynSetVol(seqp->drvr, voice, __vsVol(vs, seqp), delta);
            break;

        case AL_TREM_OSC_EVT:
            vs = seqp->nextEvent.msg.osc.vs;
            oscState = seqp->nextEvent.msg.osc.oscState;
            delta = (*seqp->updateOsc)(oscState, &oscValue);
            vs->tremelo = (u8)oscValue;
            alSynSetVol(seqp->drvr, &vs->voice, __vsVol(vs, seqp), __vsDelta(vs, seqp->curTime));
            evt.type = AL_TREM_OSC_EVT;
            evt.msg.osc.vs = vs;
            evt.msg.osc.oscState = oscState;
            alEvtqPostEvent(&seqp->evtq, &evt, delta);
            break;

        case AL_VIB_OSC_EVT:
            vs = seqp->nextEvent.msg.osc.vs;
            oscState = seqp->nextEvent.msg.osc.oscState;
            chan = seqp->nextEvent.msg.osc.chan;
            delta = (*seqp->updateOsc)(oscState, &oscValue);
            vs->vibrato = oscValue;
            alSynSetPitch(seqp->drvr, &vs->voice, vs->pitch * vs->vibrato * seqp->chanState[chan].pitchBend);
            evt.type = AL_VIB_OSC_EVT;
            evt.msg.osc.vs = vs;
            evt.msg.osc.oscState = oscState;
            evt.msg.osc.chan = chan;
            alEvtqPostEvent(&seqp->evtq, &evt, delta);
            break;

        case AL_SEQP_MIDI_EVT:
        case AL_CSP_NOTEOFF_EVT:
            __CSPHandleMIDIMsg(seqp, &seqp->nextEvent);
            break;

        case AL_SEQP_META_EVT:
            __CSPHandleMetaMsg(seqp, &seqp->nextEvent);
            break;

        case AL_SEQP_VOL_EVT:
            seqp->vol = seqp->nextEvent.msg.spvol.vol;
            for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                alSynSetVol(seqp->drvr, &vs->voice, __vsVol(vs, seqp), __vsDelta(vs, seqp->curTime));
            }
            break;

        case AL_SEQP_PLAY_EVT:
            if (seqp->state != AL_PLAYING) {
                seqp->state = AL_PLAYING;
                __CSPPostNextSeqEvent(seqp);
            }
            break;

        case AL_SEQP_STOP_EVT:
            if (seqp->state == AL_STOPPING) {
                for (vs = seqp->vAllocHead; vs != 0; vs = seqp->vAllocHead) {
                    alSynStopVoice(seqp->drvr, &vs->voice);
                    alSynFreeVoice(seqp->drvr, &vs->voice);
                    if (vs->flags) {
                        __seqpStopOsc(seqp, vs);
                    }
                    __unmapVoice(seqp, &vs->voice);
                }
                seqp->state = AL_STOPPED;
            }
            break;

        case AL_SEQP_STOPPING_EVT:
            if (seqp->state == AL_PLAYING) {
                alEvtqFlushType(&seqp->evtq, AL_SEQ_REF_EVT);
                alEvtqFlushType(&seqp->evtq, AL_CSP_NOTEOFF_EVT);
                alEvtqFlushType(&seqp->evtq, AL_SEQP_MIDI_EVT);
                for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                    if (__voiceNeedsNoteKill(seqp, &vs->voice, KILL_TIME)) {
                        __seqpReleaseVoice(seqp, &vs->voice, KILL_TIME);
                    }
                }
                seqp->state = AL_STOPPING;
                evt.type = AL_SEQP_STOP_EVT;
                alEvtqPostEvent(&seqp->evtq, &evt, AL_EVTQ_END);
            }
            break;

        case AL_SEQP_PRIORITY_EVT:
            chan = seqp->nextEvent.msg.sppriority.chan;
            seqp->chanState[chan].priority = seqp->nextEvent.msg.sppriority.priority;
            break;

        case AL_SEQP_SEQ_EVT:
            assert(seqp->state != AL_PLAYING, 295);
            seqp->target = seqp->nextEvent.msg.spseq.seq;
            __CSPSetUsptFromTempo(seqp, 500000.0);
            if (seqp->bank) {
                __initFromBank(seqp, seqp->bank);
            }
            break;

        case AL_SEQP_BANK_EVT:
            assert(seqp->state == AL_STOPPED, 304);
            seqp->bank = seqp->nextEvent.msg.spbank.bank;
            __initFromBank(seqp, seqp->bank);
            break;

        case AL_SEQ_END_EVT:
        case AL_TEMPO_EVT:
        case AL_SEQ_MIDI_EVT:
            assert(0, 314);
            break;
        }
        seqp->nextDelta = alEvtqNextEvent(&seqp->evtq, &seqp->nextEvent);
    } while (seqp->nextDelta == 0);

    seqp->curTime += seqp->nextDelta;
    return seqp->nextDelta;
}
