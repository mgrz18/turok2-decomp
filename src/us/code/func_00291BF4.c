/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_5.c, func_802B0C94_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 func_00290D90(s32 arg0);
extern const float D_800AA400;
extern const float D_800AA408;
extern char D_800AA380[];
extern char D_800AA384[];
extern void func_00291BF4(ALSeqPlayer *, ALEvent *);
extern void func_00291034(ALEventQueue *, ALEvent *, ALMicroTime);
extern void func_0029B020(char *, char *, s32);
extern const float D_800AA400;
extern const float D_800AA408;
extern char D_800AA380[];
extern char D_800AA384[];
extern ALSound_s *func_00292640(ALSeqPlayer *, u8, u8, u8);
extern ALVoiceState *func_0029247C(ALSeqPlayer *, u8, u8, u8);
extern ALVoiceState *func_002925E0(ALSeqPlayer *, u8, u8);
extern s32 alSynAllocVoice(void *, ALVoice_s *, ALVoiceConfig_s *);
extern u8 func_00292804(ALVoiceState *, ALSeqPlayer *);
extern s16 func_00292774(ALVoiceState *, ALSeqPlayer *);
extern ALMicroTime func_002927E8(ALVoiceState *, ALMicroTime);
extern void func_00294540(void *, ALVoice_s *, void *, f32, s16, u8, u8, ALMicroTime);
extern void func_00291034(ALEventQueue *, ALEvent *, ALMicroTime);
extern void func_00292A74(ALSeqPlayer *, ALVoice_s *, ALMicroTime);
extern void alSynSetVol(void *, ALVoice_s *, s16, ALMicroTime);
extern void func_002942B0(void *, ALVoice_s *, u8);
extern void func_00294200(void *, ALVoice_s *, u8);
extern void func_00294340(void *, ALVoice_s *, f32);
extern void func_00292A20(ALSeqPlayer *, ALInstrument *, s32);
extern void func_0029B020(char *, char *, s32);
void func_00291BF4(ALSeqPlayer *seqp, ALEvent *event)
{
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
    if (!(event->type == 1 || event->type == 2))
        func_0029B020(D_800AA380, D_800AA384, 436);
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
            f32 pitch, oscValue;
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
            alSynAllocVoice(seqp->drvr, voice, &config);
            vstate->sound = sound;
            vstate->envPhase = 0;
            if (seqp->chanState[chan].sustain > 63)
                vstate->phase = 2;
            else
                vstate->phase = 0;
            cents = (key - sound->keyMap->keyBase) * 100 + sound->keyMap->detune;
            vstate->pitch = func_00290D90(cents);
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
                        func_00291034(&seqp->evtq, &evt, deltaTime);
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
                        func_00291034(&seqp->evtq, &evt, deltaTime);
                        vstate->flags |= 0x02;
                    }
                }
            }
            vstate->vibrato = oscValue;
            pitch = vstate->pitch * seqp->chanState[chan].pitchBend * vstate->vibrato;
            fxmix = seqp->chanState[chan].fxmix;
            pan = func_00292804(vstate, seqp);
            vol = func_00292774(vstate, seqp);
            deltaTime = sound->envelope->attackTime;
            func_00294540(seqp->drvr, voice, sound->wavetable, pitch, vol, pan, fxmix, deltaTime);
            evt.type = 6;
            evt.msg.vol.voice = voice;
            evt.msg.vol.vol = sound->envelope->decayVolume;
            evt.msg.vol.delta = sound->envelope->decayTime;
            deltaTime = sound->envelope->attackTime;
            func_00291034(&seqp->evtq, &evt, deltaTime);
            break;
        }
    case 0x80:
        vstate = func_002925E0(seqp, key, chan);
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
        vstate = func_002925E0(seqp, key, chan);
        if (!vstate)
            return;
        vstate->velocity = byte2;
        alSynSetVol(seqp->drvr, &vstate->voice, func_00292774(vstate, seqp),
                      func_002927E8(vstate, seqp->curTime));
        break;
    case 0xD0:
        for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
            if (vs->channel == chan) {
                vs->velocity = byte1;
                alSynSetVol(seqp->drvr, &vs->voice, func_00292774(vs, seqp),
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
                    vol = func_00292774(vs, seqp);
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
            func_0029B020(D_800AA380, D_800AA384, 714);
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
