/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_13.c, func_802AF2E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 alCents2Ratio(s32 arg0);
extern char sCSPlayerFile[];
extern char D_800AA290[];
void __CSPHandleMIDIMsg(void *object, ALEvent *message);
extern float D_800AA360;
extern float D_800AA364;
extern float D_800AA368;
extern void alEvtqPostEvent(ALEventQueue *, ALEvent *, s32);
extern void __assert(void *arg0, void *arg1, s32 arg2);
extern void *__lookupSoundQuick(void *, s32, s32, s32);
extern void *__mapVoice(void *, u8, u8, u8);
extern ALVoiceState *__lookupVoice(ALSeqPlayer *, u8, u8);
extern s32 alSynAllocVoice(ALSynth *, ALVoice *, ALVoiceConfig *);
extern s32 __vsPan(void *, void *);
extern s16 __vsVol(ALVoiceState *, ALSeqPlayer *);
extern s32 __vsDelta(void *, s32);
extern void alSynStartVoiceParams(ALSynth *, ALVoice *, void *, f32, s16, u8, u8, s32);
extern void __seqpReleaseVoice(void *, void *, s32);
extern void alSynSetVol(void *, void *, s16, s32);
extern void alSynSetPan(void *, void *, u8);
extern void alSynSetFXMix(void *, void *, u8);
extern void alSynSetPitch(void *, void *, f32);
extern void __setInstChanState(void *, void *, s32);
extern void __assert(void *, void *, s32);
void __CSPHandleMIDIMsg(void *object, ALEvent *message)
{
    ALSeqPlayer *seqp = object;
    ALEvent *event = (ALEvent *)message;
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
            f32 pitch, oscValue, initialOscValue;
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
            alSynAllocVoice(seqp->drvr, (ALVoice *)voice, &config);
            vstate->sound = sound;
            vstate->envPhase = 0;
            if (seqp->chanState[chan].sustain > 63)
                vstate->phase = 2;
            else
                vstate->phase = 0;
            cents = (key - sound->keyMap->keyBase) * 100 + sound->keyMap->detune;
            vstate->pitch = alCents2Ratio(cents);
            initialOscValue = D_800AA360;
            vstate->envGain = sound->envelope->attackVolume;
            vstate->envEndTime = seqp->curTime + sound->envelope->attackTime;
            vstate->flags = 0;
            inst = seqp->chanState[chan].instrument;
            oscValue = initialOscValue;
            if (inst->tremType) {
                if (seqp->initOsc) {
                    deltaTime = (*seqp->initOsc)(&oscState, &oscValue, inst->tremType,
                                                 inst->tremRate, inst->tremDepth, inst->tremDelay);
                    if (deltaTime) {
                        evt.type = 22;
                        evt.msg.osc.vs = vstate;
                        evt.msg.osc.oscState = oscState;
                        alEvtqPostEvent(&seqp->evtq, (ALEvent *)&evt, deltaTime);
                        vstate->flags |= 0x01;
                    }
                }
            }
            tremeloSlot = &vstate->tremelo;
            if (!(oscValue >= D_800AA364)) {
                tremelo = (s32)oscValue;
            } else {
                tremelo = (s32)(oscValue - D_800AA364);
                tremelo |= 0x80000000;
            }
            initialOscValue = D_800AA368;
            *tremeloSlot = tremelo;
            oscValue = initialOscValue;
            if (inst->vibType) {
                if (seqp->initOsc) {
                    deltaTime = (*seqp->initOsc)(&oscState, &oscValue, inst->vibType,
                                                 inst->vibRate, inst->vibDepth, inst->vibDelay);
                    if (deltaTime) {
                        evt.type = 23;
                        evt.msg.osc.vs = vstate;
                        evt.msg.osc.oscState = oscState;
                        evt.msg.osc.chan = chan;
                        alEvtqPostEvent(&seqp->evtq, (ALEvent *)&evt, deltaTime);
                        vstate->flags |= 0x02;
                    }
                }
            }
            vstate->vibrato = oscValue;
            pitch = vstate->pitch * seqp->chanState[chan].pitchBend * vstate->vibrato;
            fxmix = seqp->chanState[chan].fxmix;
            pan = __vsPan(vstate, seqp);
            vol = __vsVol((ALVoiceState *)vstate, (ALSeqPlayer *)seqp);
            deltaTime = sound->envelope->attackTime;
            alSynStartVoiceParams(seqp->drvr, (ALVoice *)voice, sound->wavetable, pitch, vol, pan, fxmix, deltaTime);
            evt.type = 6;
            evt.msg.vol.voice = voice;
            evt.msg.vol.vol = sound->envelope->decayVolume;
            evt.msg.vol.delta = sound->envelope->decayTime;
            alEvtqPostEvent(&seqp->evtq, (ALEvent *)&evt, deltaTime);
            if (midi->duration) {
                evt.type = 21;
                evt.msg.midi.status = chan | 0x80;
                evt.msg.midi.byte1 = key;
                evt.msg.midi.byte2 = 0;
                deltaTime = seqp->uspt * midi->duration;
                alEvtqPostEvent(&seqp->evtq, (ALEvent *)&evt, deltaTime);
            }
            break;
        }
    case 0x80:
        vstate = (ALVoiceState *)__lookupVoice((ALSeqPlayer *)seqp, key, chan);
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
        vstate = (ALVoiceState *)__lookupVoice((ALSeqPlayer *)seqp, key, chan);
        if (!vstate)
            return;
        vstate->velocity = byte2;
        alSynSetVol(seqp->drvr, &vstate->voice, __vsVol((ALVoiceState *)vstate, (ALSeqPlayer *)seqp),
                      __vsDelta(vstate, seqp->curTime));
        break;
    case 0xD0:
        for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
            if (vs->channel == chan) {
                vs->velocity = byte1;
                alSynSetVol(seqp->drvr, &vs->voice, __vsVol((ALVoiceState *)vs, (ALSeqPlayer *)seqp),
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
                    vol = __vsVol((ALVoiceState *)vs, (ALSeqPlayer *)seqp);
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
        default:
            break;
        }
        break;
    case 0xC0:
        if (seqp->bank == 0)
            __assert(D_800AA290, sCSPlayerFile, 711);
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
