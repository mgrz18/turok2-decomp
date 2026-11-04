/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/sdk/libgultra_inferred/802B54A0_2.CPP, func_802B0554_de (libultra seqplayer.c __seqpVoiceHandler), with Turok 2's layouts. */
#include "common.h"
#include "structs/libaudio.h"

extern s32 func_00292530(void *arg0, s32 arg1, s32 arg2);
extern void func_00292B94(void *arg0, void *arg1);
struct Node_func_80239AF4_de;
typedef struct Node_func_80239AF4_de Node_func_80239AF4_de;
struct Node_func_80239AF4_de;
struct Node_func_80239AF4_de {
    struct Node_func_80239AF4_de *next;
};
struct Shape_typemap_71;
struct Shape_typemap_71 {
    unsigned char padding_0[28];
    int field_1C;
};
struct List802B663C;
typedef struct List802B663C List802B663C;
struct List802B663C;
struct List802B663C {
    char pad[0x64];
    Node_func_80239AF4_de *head;
    Node_func_80239AF4_de *tail;
    Node_func_80239AF4_de *free;
};
extern void func_002924CC(List802B663C *arg0, void *arg1);
extern void func_00292C84(void *arg0);
extern void func_0029B020(const char *arg0, const char *arg1, s32 arg2);
extern char D_800AA380[];
extern char D_800AA384[];
extern void func_002919F0(ALSeqPlayer *);
extern void func_00291BF4(ALSeqPlayer *, ALEvent *);
extern void func_00292D4C(ALSeqPlayer *, ALEvent *);
extern void func_00292DB0(ALSeqPlayer *, f32);
extern void func_00291034(ALEventQueue *, ALEvent *, ALMicroTime);
extern ALMicroTime func_00290F9C(ALEventQueue *, ALEvent *);
extern void func_002911B8(ALEventQueue *, s16);
extern void alSynStopVoice(struct Shape_typemap_71 *, ALVoice *);
extern void func_00294160(void *, ALVoice *);
extern void alSynSetVol(void *, ALVoice *, s16, ALMicroTime);
extern void func_00294340(void *, ALVoice *, f32);
extern s16 func_00292774(ALVoiceState *, ALSeqPlayer *);
extern ALMicroTime func_002927E8(ALVoiceState *, ALMicroTime);
extern void func_00292A74(ALSeqPlayer *, ALVoice *, ALMicroTime);
extern void func_00292844(ALSeqPlayer *, void *);
ALMicroTime func_002914B4(void *node)
{
    ALSeqPlayer *seqp = node;
    ALEvent evt;
    ALVoice *voice;
    ALMicroTime delta;
    ALVoiceState *vs;
    void *oscState;
    f32 oscValue;
    u8 chan;
    u32 tremelo;
    u8 *tremeloSlot;
    do {
        switch (seqp->nextEvent.type) {
        case 0:
            func_002919F0(seqp);
            break;
        case 9:
            evt.type = 9;
            func_00291034(&seqp->evtq, (ALEvent *)&evt, seqp->frameTime);
            break;
        case 5:
            voice = seqp->nextEvent.msg.note.voice;
            alSynStopVoice(seqp->drvr, voice);
            func_00294160(seqp->drvr, voice);
            vs = (ALVoiceState *)voice->clientPrivate;
            if (vs->flags)
                func_00292B94(seqp, vs);
            func_002924CC(seqp, voice);
            break;
        case 6:
            voice = seqp->nextEvent.msg.vol.voice;
            vs = (ALVoiceState *)voice->clientPrivate;
            if (vs->envPhase == 0)
                vs->envPhase = 1;
            delta = seqp->nextEvent.msg.vol.delta;
            vs->envGain = seqp->nextEvent.msg.vol.vol;
            vs->envEndTime = seqp->curTime + delta;
            alSynSetVol(seqp->drvr, voice, func_00292774(vs, seqp), delta);
            break;
        case 22:
            vs = seqp->nextEvent.msg.osc.vs;
            oscState = seqp->nextEvent.msg.osc.oscState;
            delta = (*seqp->updateOsc)(oscState, &oscValue);
            tremeloSlot = &vs->tremelo;
            tremelo = (u32)oscValue;
            *tremeloSlot = tremelo;
            alSynSetVol(seqp->drvr, &vs->voice, func_00292774(vs, seqp),
                          func_002927E8(vs, seqp->curTime));
            evt.type = 22;
            evt.msg.osc.vs = vs;
            evt.msg.osc.oscState = oscState;
            func_00291034(&seqp->evtq, &evt, delta);
            break;
        case 23:
            vs = seqp->nextEvent.msg.osc.vs;
            oscState = seqp->nextEvent.msg.osc.oscState;
            chan = seqp->nextEvent.msg.osc.chan;
            delta = (*seqp->updateOsc)(oscState, &oscValue);
            vs->vibrato = oscValue;
            func_00294340(seqp->drvr, &vs->voice,
                          vs->pitch * vs->vibrato * seqp->chanState[chan].pitchBend);
            evt.type = 23;
            evt.msg.osc.vs = vs;
            evt.msg.osc.oscState = oscState;
            evt.msg.osc.chan = chan;
            func_00291034(&seqp->evtq, &evt, delta);
            break;
        case 2:
            func_00291BF4(seqp, &seqp->nextEvent);
            break;
        case 7:
            func_00292D4C(seqp, &seqp->nextEvent);
            break;
        case 15:
            if (seqp->state != 1) {
                seqp->state = 1;
                func_00292C84(seqp);
            }
            break;
        case 16:
            if (seqp->state == 2) {
                for (vs = seqp->vAllocHead; vs != 0; vs = seqp->vAllocHead) {
                    alSynStopVoice(seqp->drvr, &vs->voice);
                    func_00294160(seqp->drvr, &vs->voice);
                    if (vs->flags)
                        func_00292B94(seqp, vs);
                    func_002924CC(seqp, &vs->voice);
                }
                seqp->curTime = 0;
                seqp->state = 0;
            }
            break;
        case 17:
            if (seqp->state == 1) {
                func_002911B8(&seqp->evtq, 0);
                func_002911B8(&seqp->evtq, 2);
                for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                    if ((u8)func_00292530(seqp, &vs->voice, 50000))
                        func_00292A74(seqp, &vs->voice, 50000);
                }
                seqp->state = 2;
                evt.type = 16;
                func_00291034(&seqp->evtq, &evt, 0x7fffffff);
            }
            break;
        case 10:
            seqp->vol = seqp->nextEvent.msg.spvol.vol;
            for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                alSynSetVol(seqp->drvr, &vs->voice, func_00292774(vs, seqp),
                              func_002927E8(vs, seqp->curTime));
            }
            break;
        case 11:
            seqp->loopStart = seqp->nextEvent.msg.loop.start;
            seqp->loopEnd = seqp->nextEvent.msg.loop.end;
            seqp->loopCount = seqp->nextEvent.msg.loop.count;
            break;
        case 12:
            chan = seqp->nextEvent.msg.sppriority.chan;
            seqp->chanState[chan].priority = seqp->nextEvent.msg.sppriority.priority;
            break;
        case 13:
            if (seqp->state == 1)
                func_0029B020(D_800AA380, D_800AA384, 296);
            seqp->target = seqp->nextEvent.msg.spseq.seq;
            func_00292DB0(seqp, 500000.0f);
            if (seqp->bank)
                func_00292844(seqp, seqp->bank);
            break;
        case 14:
            if (seqp->state != 0)
                func_0029B020(D_800AA380, D_800AA384, 305);
            seqp->bank = seqp->nextEvent.msg.spbank.bank;
            func_00292844(seqp, seqp->bank);
            break;
        case 4:
        case 3:
        case 1:
            func_0029B020(D_800AA380, D_800AA384, 315);
            break;
        }
        seqp->nextDelta = func_00290F9C(&seqp->evtq, &seqp->nextEvent);
    } while (seqp->nextDelta == 0);
    seqp->curTime += seqp->nextDelta;
    return seqp->nextDelta;
}
