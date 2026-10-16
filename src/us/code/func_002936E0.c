/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_5.c, func_802B2780_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct ALEventQueue;
typedef struct ALEventQueue ALEventQueue;
struct ALNoteEvent;
struct ALOscEvent;
struct ALVolumeEvent;
struct ALEndEvent;
struct ALTempoEvent;
struct ALEvent;
typedef struct ALNoteEvent ALNoteEvent;
typedef struct ALOscEvent ALOscEvent;
typedef struct ALVolumeEvent ALVolumeEvent;
typedef struct ALEndEvent ALEndEvent;
typedef struct ALTempoEvent ALTempoEvent;
typedef struct ALEvent ALEvent;
struct ListLink;
struct ListLink {
    struct ListLink *next;
    struct ListLink *prev;
};
struct ListLink;
typedef struct ListLink ListLink;
struct ALEventQueue;
struct ALEventQueue {
    ListLink freeList;
    ListLink allocList;
    s32 eventCount;
};
struct ALEvent;
struct ALVoice_s;
typedef struct ALVoice_s ALVoice_s;
struct ALVoice_s;
struct ALVoice_s {
    ListLink node;
    void *pvoice;
    void *table;
    void *clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
};
struct ALSynth_func_802B3000_de;
typedef struct ALSynth_func_802B3000_de ALSynth_func_802B3000_de;
struct ALMIDIEvent;
typedef struct ALMIDIEvent ALMIDIEvent;
struct InventorySlot;
struct InventorySlot {
    u8 available;
    u8 slot;
};
struct InventorySlot;
typedef struct InventorySlot InventorySlot;
struct ALMIDIEvent;
struct ALMIDIEvent {
    s32 ticks;
    u8 status;
    u8 byte1;
    u8 byte2;
    u32 duration;
};
struct ALSynth_func_802B3000_de;
struct ALSynth_func_802B3000_de {
    void *head;
    ListLink pFreeList;
    ListLink pAllocList;
    ListLink pLameList;
    s32 paramSamples;
};
struct ALVoiceConfig_s;
typedef struct ALVoiceConfig_s ALVoiceConfig_s;
struct ALPlayer_s;
typedef struct ALPlayer_s ALPlayer_s;
struct ALPlayer_s;
struct ALPlayer_s {
    struct ALPlayer_s *next;
    void *clientData;
    void *handler;
    s32 callTime;
    s32 samplesLeft;
};
struct Request;
struct Request {
    s16 type;
};
struct func_80284AF4_G2;
typedef struct func_80284AF4_G2 func_80284AF4_G2;
struct Request;
typedef struct Request Request;
struct func_80284AF4_G2;
struct func_80284AF4_G2 {
    void * unk0;
};
struct ALEndEvent {
    s32 ticks;
    u8 status;
    u8 type;
    u8 len;
};
struct ALTempoEvent {
    s32 ticks;
    u8 status;
    u8 type;
    u8 len;
    u8 byte1;
    u8 byte2;
    u8 byte3;
};
struct ALNoteEvent {
    struct ALVoice_s *voice;
};
struct ALVolumeEvent {
    struct ALVoice_s *voice;
    s32 delta;
    u8 vol;
};
struct ALOscEvent {
    struct ALVoiceState *vs;
    void *oscState;
    u8 chan;
};
struct ALEvent {
    s16 type;
    union {
        ALMIDIEvent midi;
        ALTempoEvent tempo;
        ALEndEvent end;
        ALNoteEvent note;
        ALVolumeEvent vol;
        Request spvol;
        InventorySlot sppriority;
        func_80284AF4_G2 spseq;
        func_80284AF4_G2 spbank;
        ALOscEvent osc;
    } msg;
};
struct ALVoiceConfig_s;
struct ALVoiceConfig_s {
    s16 priority;
    s16 fxBus;
    u8 unityPitch;
};
struct ALEvent;
struct ALEndEvent;
struct ALTempoEvent;
struct ALEvent;
union ALSndpEvent;
union ALSndpEvent {
    ALEvent msg;
    struct {
        s16 type;
        void *state;
    } common;
};
struct ALSndPlayer;
struct ALSndPlayer {
    ALPlayer_s node;
    ALEventQueue evtq;
    ALEvent nextEvent;
    void *drvr;
    s32 target;
    void *sndState;
    s32 maxSounds;
    s32 frameTime;
    s32 nextDelta;
    s32 curTime;
};
struct ALSndPlayer;
union ALSndpEvent;
struct ALSndPlayer;
typedef struct ALSndPlayer ALSndPlayer;
union ALSndpEvent;
typedef union ALSndpEvent ALSndpEvent;
s32 func_00293C50(s32 val, f32 div);
extern const f32 D_800AA484;
extern void func_002936E0(struct ALSndPlayer * arg0, union ALSndpEvent * arg1);
struct ALKeyMap;
struct ALKeyMap {
    u8 velocityMin;
    u8 velocityMax;
    u8 keyMin;
    u8 keyMax;
    u8 keyBase;
    s8 detune;
};
struct ALVoiceState;
struct ALSound_s;
typedef struct ALSound_s ALSound_s;
struct ALKeyMap;
typedef struct ALKeyMap ALKeyMap;
struct ALEnvelope;
struct ALEnvelope {
    s32 attackTime;
    s32 decayTime;
    s32 releaseTime;
    u8 attackVolume;
    u8 decayVolume;
};
struct ALEnvelope;
typedef struct ALEnvelope ALEnvelope;
struct ALSound_s;
struct ALSound_s {
    ALEnvelope *envelope;
    ALKeyMap *keyMap;
    void *wavetable;
    u8 samplePan;
    u8 sampleVolume;
    u8 flags;
};
struct ALVoiceState;
typedef struct ALVoiceState ALVoiceState;
struct ALVoice_s;
struct ALVoiceState;
struct ALVoiceState {
    struct ALVoiceState *next;
    ALVoice_s voice;
    ALSound_s *sound;
    s32 envEndTime;
    f32 pitch;
    f32 vibrato;
    u8 envGain;
    u8 channel;
    u8 key;
    u8 velocity;
    u8 envPhase;
    u8 phase;
    u8 tremelo;
    u8 flags;
};
struct ALOscEvent;
struct ALVoiceState;
struct ALVoiceState;
struct ALVoiceState;
struct ALVoiceState;
struct ALVoiceState;
struct ALNoteEvent;
struct ALOscEvent;
struct ALVolumeEvent;
struct ALNoteEvent;
struct ALVoice_s;
struct ALEvent;
struct ALEvent;
union ALSndpEvent_func_802B2780_de;
typedef union ALSndpEvent_func_802B2780_de ALSndpEvent_func_802B2780_de;
struct ALSoundState_func_802B2780_de;
typedef struct ALSoundState_func_802B2780_de ALSoundState_func_802B2780_de;
struct ALSound_s_func_802B2780_de;
typedef struct ALSound_s_func_802B2780_de ALSound_s_func_802B2780_de;
struct ALSound_s_func_802B2780_de;
struct ALSound_s_func_802B2780_de {
    ALEnvelope *envelope;
    void *keyMap;
    void *wavetable;
    u8 samplePan;
    u8 sampleVolume;
    u8 flags;
};
struct ALSoundState_func_802B2780_de;
struct ALSound_s_func_802B2780_de;
struct ALSoundState_func_802B2780_de {
    ALVoice_s voice;
    struct ALSound_s_func_802B2780_de *sound;
    s16 priority;
    f32 pitch;
    s32 state;
    s16 vol;
    u8 pan;
    u8 fxMix;
};
union ALSndpEvent_func_802B2780_de;
union ALSndpEvent_func_802B2780_de {
    ALEvent msg;
    struct {
        s16 type;
        ALSoundState_func_802B2780_de *state;
    } common;
    struct {
        s16 type;
        ALSoundState_func_802B2780_de *state;
        s16 vol;
    } vol;
    struct {
        s16 type;
        ALSoundState_func_802B2780_de *state;
        f32 pitch;
    } pitch;
    struct {
        s16 type;
        ALSoundState_func_802B2780_de *state;
        u8 pan;
    } pan;
    struct {
        s16 type;
        ALSoundState_func_802B2780_de *state;
        u8 mix;
    } fx;
};
struct ALSoundState;
typedef struct ALSoundState ALSoundState;
struct ALSoundState;
struct ALSoundState {
    ALVoice_s voice;
    void *sound;
    s16 priority;
    f32 pitch;
    s32 state;
    s16 vol;
    u8 pan;
    u8 fxMix;
};
extern void func_00291034(ALEventQueue *, ALEvent *, s32);
extern s32 func_00293F60(ALSynth_func_802B3000_de *, ALVoice_s *, ALVoiceConfig_s *);
extern void func_002944A0(ALSynth_func_802B3000_de *, ALVoice_s *, void *);
extern void func_002942B0(ALSynth_func_802B3000_de *, ALVoice_s *, u8);
extern void func_002943E0(ALSynth_func_802B3000_de *, ALVoice_s *, s16, s32);
extern void func_00294340(ALSynth_func_802B3000_de *, ALVoice_s *, f32);
extern void func_00294200(ALSynth_func_802B3000_de *, ALVoice_s *, u8);
extern void func_00294650(ALSynth_func_802B3000_de *, ALVoice_s *);
extern void func_00294160(ALSynth_func_802B3000_de *, ALVoice_s *);
extern void func_00293BA8(ALEventQueue *, ALSoundState *);
void func_002936E0(ALSndPlayer *sndp, ALSndpEvent *event)
{
    ALVoiceConfig_s vc;
    ALSound_s *snd;
    ALVoice_s *voice;
    u8 pan;
    f32 pitch;
    ALSndpEvent evt;
    s32 delta;
    s16 vol;
    s16 tmp;
    s32 vtmp;
    ALSoundState *state;
    const ALSndpEvent_func_802B2780_de *event_view =
        (const ALSndpEvent_func_802B2780_de *)event;
    state = event->common.state;
    snd = state->sound;
    switch (event->msg.type) {
        case 0:
            if (state->state != 0 || !snd)
                return;
            vc.fxBus = 0;
            vc.priority = state->priority;
            vc.unityPitch = 0;
            voice = &state->voice;
            func_00293F60(sndp->drvr, voice, &vc);
            vol = (s16)((s32)snd->envelope->attackVolume * state->vol / 127);
            tmp = state->pan - 64 + ((const ALSound_s_func_802B2780_de *)snd)->samplePan;
            tmp = ((tmp) > (0) ? (tmp) : (0));
            pan = (u8)((tmp) < (127) ? (tmp) : (127));
            pitch = state->pitch;
            delta = snd->envelope->attackTime;
            func_002944A0(sndp->drvr, voice, snd->wavetable);
            state->state = 1;
            func_002942B0(sndp->drvr, voice, pan);
            func_002943E0(sndp->drvr, voice, vol, delta);
            func_00294340(sndp->drvr, voice, pitch);
            func_00294200(sndp->drvr, voice, state->fxMix);
            evt.common.type = 6;
            evt.common.state = state;
            delta = (s32)func_00293C50(snd->envelope->attackTime, state->pitch);
            func_00291034(&sndp->evtq, (ALEvent *)&evt, delta);
            break;
        case 1:
            if (state->state != 1 || !snd)
                return;
            delta = (s32)func_00293C50(snd->envelope->releaseTime, state->pitch);
            func_002943E0(sndp->drvr, &state->voice, 0, delta);
            if (delta) {
                evt.common.type = 7;
                evt.common.state = state;
                func_00291034(&sndp->evtq, (ALEvent *)&evt, delta);
                state->state = 2;
            } else {
                func_00294650(sndp->drvr, &state->voice);
                func_00294160(sndp->drvr, &state->voice);
                func_00293BA8(&sndp->evtq, state);
                state->state = 0;
            }
            break;
        case 2:
            state->pan = event_view->pan.pan;
            if (state->state == 1 && snd) {
                tmp = state->pan - 64 + ((const ALSound_s_func_802B2780_de *)snd)->samplePan;
                tmp = ((tmp) > (0) ? (tmp) : (0));
                pan = (u8)((tmp) < (127) ? (tmp) : (127));
                func_002942B0(sndp->drvr, &state->voice, pan);
            }
            break;
        case 4:
            {
                f32 min = D_800AA484;
                if ((state->pitch = event_view->pitch.pitch) < min)
                    state->pitch = min;
            }
            if (state->state == 1) {
                func_00294340(sndp->drvr, &state->voice, state->pitch);
            }
            break;
        case 8:
            state->fxMix = event_view->fx.mix;
            if (state->state == 1)
                func_00294200(sndp->drvr, &state->voice, state->fxMix);
            break;
        case 3:
            state->vol = event_view->vol.vol;
            if (state->state == 1 && snd) {
                vtmp = snd->envelope->decayVolume * state->vol / 127;
                func_002943E0(sndp->drvr, &state->voice, (s16)vtmp, 1000);
            }
            break;
        case 6:
            if (snd->envelope->decayTime != -1) {
                vtmp = snd->envelope->decayVolume * state->vol / 127;
                delta = (s32)func_00293C50(snd->envelope->decayTime, state->pitch);
                func_002943E0(sndp->drvr, &state->voice, (s16)vtmp, delta);
                evt.common.type = 1;
                evt.common.state = state;
                func_00291034(&sndp->evtq, (ALEvent *)&evt, delta);
            }
            break;
        case 7:
            func_00294650(sndp->drvr, &state->voice);
            func_00294160(sndp->drvr, &state->voice);
            func_00293BA8(&sndp->evtq, state);
            state->state = 0;
            break;
        default:
            break;
    }
}
