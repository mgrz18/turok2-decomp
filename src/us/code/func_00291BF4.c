/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_5.c, func_802B0C94_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 alCents2Ratio(s32 arg0);
extern const float D_800AA400;
extern const float D_800AA408;
extern char D_800AA380[];
extern char D_800AA384[];
extern void __handleMIDIMsg(ALSeqPlayer *, ALEvent *);
extern void alEvtqPostEvent(ALEventQueue *, ALEvent *, ALMicroTime);
extern void __assert(char *, char *, s32);
extern const float D_800AA400;
extern const float D_800AA408;
extern char D_800AA380[];
extern char D_800AA384[];
extern ALSound *__lookupSoundQuick(ALSeqPlayer *, u8, u8, u8);
extern ALVoiceState *__mapVoice(ALSeqPlayer *, u8, u8, u8);
extern ALVoiceState *__lookupVoice(ALSeqPlayer *, u8, u8);
extern s32 alSynAllocVoice(void *, ALVoice *, ALVoiceConfig *);
extern u8 __vsPan(ALVoiceState *, ALSeqPlayer *);
extern s16 __vsVol(ALVoiceState *, ALSeqPlayer *);
extern ALMicroTime __vsDelta(ALVoiceState *, ALMicroTime);
extern void alSynStartVoiceParams(void *, ALVoice *, void *, f32, s16, u8, u8, ALMicroTime);
extern void alEvtqPostEvent(ALEventQueue *, ALEvent *, ALMicroTime);
extern void __seqpReleaseVoice(ALSeqPlayer *, ALVoice *, ALMicroTime);
extern void alSynSetVol(void *, ALVoice *, s16, ALMicroTime);
extern void alSynSetPan(void *, ALVoice *, u8);
extern void alSynSetFXMix(void *, ALVoice *, u8);
extern void alSynSetPitch(void *, ALVoice *, f32);
extern void __setInstChanState(ALSeqPlayer *, ALInstrument *, s32);
extern void __assert(char *, char *, s32);
void __handleMIDIMsg(ALSeqPlayer *seqp, ALEvent *event)
{
    ALVoice *voice;
    ALVoiceState *vs;
    s32 status;
    u8 chan;
    u8 key;
    u8 vel;
    u8 byte1;
    u8 byte2;
    ALMIDIEvent *midi = &event->msg.midi;
    s16 vol;
    ALEvent evt;
    ALMicroTime deltaTime;
    ALVoiceState *vstate;
    u8 pan;
    if (!(event->type == 1 || event->type == 2))
        __assert(D_800AA380, D_800AA384, 436);
    status = event->msg.midi.status & 0xF0;
    chan = event->msg.midi.status & 0x0F;
    byte1 = key = event->msg.midi.byte1;
    byte2 = vel = event->msg.midi.byte2;
    switch (status) {
    case 0x90:
        if (vel != 0) {
            ALVoiceConfig config;
            ALSound *sound;
            s16 cents;
            f32 pitch, oscValue;
            u8 fxmix;
            void *oscState;
            ALInstrument *inst;
            s32 tremelo;
            u8 *tremeloSlot;
            if (seqp->state != 1)
                break;
            sound = __lookupSoundQuick(seqp, key, vel, chan);
            if (!sound)
                return;
            config.priority = seqp->chanState[chan].priority;
            config.fxBus = 0;
            config.unityPitch = 0;
            vstate = __mapVoice(seqp, key, vel, chan);
            if (!vstate)
                return;
            voice = &vstate->voice;
            alSynAllocVoice(seqp->drvr, voice, &config);
            vstate->sound = sound;
            vstate->envPhase = 0;
            if (seqp->chanState[chan].sustain > 63)
                vstate->phase = 2;
            else
                vstate->phase = 0;
            cents = (key - sound->keyMap->keyBase) * 100 + sound->keyMap->detune;
            vstate->pitch = alCents2Ratio(cents);
            vstate->envGain = sound->envelope->attackVolume;
            vstate->envEndTime = seqp->curTime + sound->envelope->attackTime;
            vstate->flags = 0;
            inst = seqp->chanState[chan].instrument;
            oscValue = D_800AA400;
            if (inst->tremType) {
                if (seqp->initOsc) {
                    deltaTime = (*seqp->initOsc)(&oscState, &oscValue, inst->tremType,
                                                 inst->tremRate, inst->tremDepth, inst->tremDelay);
                    if (deltaTime) {
                        evt.type = 22;
                        evt.msg.osc.vs = vstate;
                        evt.msg.osc.oscState = oscState;
                        alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);
                        vstate->flags |= 0x01;
                    }
                }
            }
            tremeloSlot = &vstate->tremelo;
            if (!(oscValue >= (*(&D_800AA400 + 1)))) {
                tremelo = (s32)oscValue;
            } else {
                tremelo = (s32)(oscValue - (*(&D_800AA400 + 1)));
                tremelo |= 0x80000000;
            }
            *tremeloSlot = tremelo;
            oscValue = D_800AA408;
            if (inst->vibType) {
                if (seqp->initOsc) {
                    deltaTime = (*seqp->initOsc)(&oscState, &oscValue, inst->vibType,
                                                 inst->vibRate, inst->vibDepth, inst->vibDelay);
                    if (deltaTime) {
                        evt.type = 23;
                        evt.msg.osc.vs = vstate;
                        evt.msg.osc.oscState = oscState;
                        evt.msg.osc.chan = chan;
                        alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);
                        vstate->flags |= 0x02;
                    }
                }
            }
            vstate->vibrato = oscValue;
            pitch = vstate->pitch * seqp->chanState[chan].pitchBend * vstate->vibrato;
            fxmix = seqp->chanState[chan].fxmix;
            pan = __vsPan(vstate, seqp);
            vol = __vsVol(vstate, seqp);
            deltaTime = sound->envelope->attackTime;
            alSynStartVoiceParams(seqp->drvr, voice, sound->wavetable, pitch, vol, pan, fxmix, deltaTime);
            evt.type = 6;
            evt.msg.vol.voice = voice;
            evt.msg.vol.vol = sound->envelope->decayVolume;
            evt.msg.vol.delta = sound->envelope->decayTime;
            deltaTime = sound->envelope->attackTime;
            alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);
            break;
        }
    case 0x80:
        vstate = __lookupVoice(seqp, key, chan);
        if (!vstate)
            return;
        if (vstate->phase == 2)
            vstate->phase = 4;
        else {
            vstate->phase = 3;
            __seqpReleaseVoice(seqp, &vstate->voice, vstate->sound->envelope->releaseTime);
        }
        break;
    case 0xA0:
        vstate = __lookupVoice(seqp, key, chan);
        if (!vstate)
            return;
        vstate->velocity = byte2;
        alSynSetVol(seqp->drvr, &vstate->voice, __vsVol(vstate, seqp),
                      __vsDelta(vstate, seqp->curTime));
        break;
    case 0xD0:
        for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
            if (vs->channel == chan) {
                vs->velocity = byte1;
                alSynSetVol(seqp->drvr, &vs->voice, __vsVol(vs, seqp),
                              __vsDelta(vs, seqp->curTime));
            }
        }
        break;
    case 0xB0:
        switch (byte1) {
        case 0x0A:
            seqp->chanState[chan].pan = byte2;
            for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                if (vs->channel == chan) {
                    pan = __vsPan(vs, seqp);
                    alSynSetPan(seqp->drvr, &vs->voice, pan);
                }
            }
            break;
        case 0x07:
            seqp->chanState[chan].vol = byte2;
            for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                if ((vs->channel == chan) && (vs->envPhase != 3)) {
                    vol = __vsVol(vs, seqp);
                    alSynSetVol(seqp->drvr, &vs->voice, vol, __vsDelta(vs, seqp->curTime));
                }
            }
            break;
        case 0x10:
            seqp->chanState[chan].priority = byte2;
            break;
        case 0x40:
            seqp->chanState[chan].sustain = byte2;
            for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                if ((vs->channel == chan) && (vs->phase != 3)) {
                    if (byte2 > 63) {
                        if (vs->phase == 0)
                            vs->phase = 2;
                    } else {
                        if (vs->phase == 2)
                            vs->phase = 0;
                        else if (vs->phase == 4) {
                            vs->phase = 3;
                            __seqpReleaseVoice(seqp, &vs->voice, vs->sound->envelope->releaseTime);
                        }
                    }
                }
            }
            break;
        case 0x5B:
            seqp->chanState[chan].fxmix = byte2;
            for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                if (vs->channel == chan)
                    alSynSetFXMix(seqp->drvr, &vs->voice, byte2);
            }
            break;
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x5D:
        default:
            break;
        }
        break;
    case 0xC0:
        if (seqp->bank == 0)
            __assert(D_800AA380, D_800AA384, 714);
        if (key < seqp->bank->instCount) {
            ALInstrument *inst = seqp->bank->instArray[key];
            __setInstChanState(seqp, inst, chan);
        }
        break;
    case 0xE0:
        {
            s32 bendVal;
            f32 bendRatio;
            s32 cents;
            bendVal = ((byte2 << 7) + byte1) - 8192;
            cents = (seqp->chanState[chan].bendRange * bendVal) / 8192;
            bendRatio = alCents2Ratio(cents);
            seqp->chanState[chan].pitchBend = bendRatio;
            for (vs = seqp->vAllocHead; vs != 0; vs = vs->next)
                if (vs->channel == chan)
                    alSynSetPitch(seqp->drvr, &vs->voice, vs->pitch * bendRatio * vs->vibrato);
        }
        break;
    default:
        break;
    }
}
