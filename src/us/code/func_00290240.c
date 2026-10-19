/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_13.c, func_802AF2E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 func_00290D90(s32 arg0);
extern char D_800AA294[];
extern char D_800AA290[];
void func_00290240(void *object, ALEvent *message);
extern float D_800AA360;
extern float D_800AA364;
extern float D_800AA368;
extern void func_00291034(ALEventQueue *, ALEvent *, s32);
extern void func_0029B020(void *arg0, void *arg1, s32 arg2);
extern void *func_00292640(void *, s32, s32, s32);
extern void *func_0029247C(void *, u8, u8, u8);
extern ALVoiceState *func_002925E0(ALSeqPlayer *, u8, u8);
extern s32 alSynAllocVoice(ALSynth_func_802B3000_de *, ALVoice_s_func_802B3000_de *, ALVoiceConfig_s *);
extern s32 func_00292804(void *, void *);
extern s16 func_00292774(ALVoiceState *, ALSeqPlayer *);
extern s32 func_002927E8(void *, s32);
extern void func_00294540(ALSynth_func_802B3000_de *, ALVoice_s_func_802B3000_de *, void *, f32, s16, u8, u8, s32);
extern void func_00292A74(void *, void *, s32);
extern void alSynSetVol(void *, void *, s16, s32);
extern void func_002942B0(void *, void *, u8);
extern void func_00294200(void *, void *, u8);
extern void func_00294340(void *, void *, f32);
extern void func_00292A20(void *, void *, s32);
extern void func_0029B020(void *, void *, s32);
void func_00290240(void *object, ALEvent *message)
{
    ALSeqPlayer *seqp = object;
    ALEvent *event = (ALEvent *)message;
    ALVoice_s *voice;
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
            ALVoiceConfig_s config;
            ALSound_s *sound;
            s16 cents;
            f32 pitch, oscValue, initialOscValue;
            u8 fxmix;
            void *oscState;
            ALInstrument *inst;
            s32 tremelo;
            u8 *tremeloSlot;
            if (seqp->state != 1)
                break;
            sound = func_00292640(seqp, key, vel, chan);
            if (!sound)
                return;
            config.priority = seqp->chanState[chan].priority;
            config.fxBus = 0;
            config.unityPitch = 0;
            vstate = func_0029247C(seqp, key, vel, chan);
            if (!vstate)
                return;
            voice = &vstate->voice;
            alSynAllocVoice(seqp->drvr, (ALVoice_s_func_802B3000_de *)voice, &config);
            vstate->sound = sound;
            vstate->envPhase = 0;
            if (seqp->chanState[chan].sustain > 63)
                vstate->phase = 2;
            else
                vstate->phase = 0;
            cents = (key - sound->keyMap->keyBase) * 100 + sound->keyMap->detune;
            vstate->pitch = func_00290D90(cents);
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
                        func_00291034(&seqp->evtq, (ALEvent *)&evt, deltaTime);
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
                        func_00291034(&seqp->evtq, (ALEvent *)&evt, deltaTime);
                        vstate->flags |= 0x02;
                    }
                }
            }
            vstate->vibrato = oscValue;
            pitch = vstate->pitch * seqp->chanState[chan].pitchBend * vstate->vibrato;
            fxmix = seqp->chanState[chan].fxmix;
            pan = func_00292804(vstate, seqp);
            vol = func_00292774((ALVoiceState *)vstate, (ALSeqPlayer *)seqp);
            deltaTime = sound->envelope->attackTime;
            func_00294540(seqp->drvr, (ALVoice_s_func_802B3000_de *)voice, sound->wavetable, pitch, vol, pan, fxmix, deltaTime);
            evt.type = 6;
            evt.msg.vol.voice = voice;
            evt.msg.vol.vol = sound->envelope->decayVolume;
            evt.msg.vol.delta = sound->envelope->decayTime;
            func_00291034(&seqp->evtq, (ALEvent *)&evt, deltaTime);
            if (midi->duration) {
                evt.type = 21;
                evt.msg.midi.status = chan | 0x80;
                evt.msg.midi.byte1 = key;
                evt.msg.midi.byte2 = 0;
                deltaTime = seqp->uspt * midi->duration;
                func_00291034(&seqp->evtq, (ALEvent *)&evt, deltaTime);
            }
            break;
        }
    case 0x80:
        vstate = (ALVoiceState *)func_002925E0((ALSeqPlayer *)seqp, key, chan);
        if (!vstate)
            return;
        if (vstate->phase == 2)
            vstate->phase = 4;
        else {
            vstate->phase = 3;
            func_00292A74(seqp, &vstate->voice, vstate->sound->envelope->releaseTime);
        }
        break;
    case 0xA0:
        vstate = (ALVoiceState *)func_002925E0((ALSeqPlayer *)seqp, key, chan);
        if (!vstate)
            return;
        vstate->velocity = byte2;
        alSynSetVol(seqp->drvr, &vstate->voice, func_00292774((ALVoiceState *)vstate, (ALSeqPlayer *)seqp),
                      func_002927E8(vstate, seqp->curTime));
        break;
    case 0xD0:
        for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
            if (vs->channel == chan) {
                vs->velocity = byte1;
                alSynSetVol(seqp->drvr, &vs->voice, func_00292774((ALVoiceState *)vs, (ALSeqPlayer *)seqp),
                              func_002927E8(vs, seqp->curTime));
            }
        }
        break;
    case 0xB0:
        switch (byte1) {
        case 0x0A:
            seqp->chanState[chan].pan = byte2;
            for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                if (vs->channel == chan) {
                    pan = func_00292804(vs, seqp);
                    func_002942B0(seqp->drvr, &vs->voice, pan);
                }
            }
            break;
        case 0x07:
            seqp->chanState[chan].vol = byte2;
            for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                if ((vs->channel == chan) && (vs->envPhase != 3)) {
                    vol = func_00292774((ALVoiceState *)vs, (ALSeqPlayer *)seqp);
                    alSynSetVol(seqp->drvr, &vs->voice, vol, func_002927E8(vs, seqp->curTime));
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
                            func_00292A74(seqp, &vs->voice, vs->sound->envelope->releaseTime);
                        }
                    }
                }
            }
            break;
        case 0x5B:
            seqp->chanState[chan].fxmix = byte2;
            for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                if (vs->channel == chan)
                    func_00294200(seqp->drvr, &vs->voice, byte2);
            }
            break;
        default:
            break;
        }
        break;
    case 0xC0:
        if (seqp->bank == 0)
            func_0029B020(D_800AA290, D_800AA294, 711);
        if (key < seqp->bank->instCount) {
            ALInstrument *inst = seqp->bank->instArray[key];
            func_00292A20(seqp, inst, chan);
        }
        break;
    case 0xE0:
        {
            s32 bendVal;
            f32 bendRatio;
            s32 cents;
            bendVal = ((byte2 << 7) + byte1) - 8192;
            cents = (seqp->chanState[chan].bendRange * bendVal) / 8192;
            bendRatio = func_00290D90(cents);
            seqp->chanState[chan].pitchBend = bendRatio;
            for (vs = seqp->vAllocHead; vs != 0; vs = vs->next)
                if (vs->channel == chan)
                    func_00294340(seqp->drvr, &vs->voice, vs->pitch * bendRatio * vs->vibrato);
        }
        break;
    default:
        break;
    }
}
