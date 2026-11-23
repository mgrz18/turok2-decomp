/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/synaddplayer.c, func_802B3000_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/libaudio.h"

extern s32 _allocatePVoice(ALSynth *, PVoice **, s16);
extern ALParam *__allocParam(void);
s32 alSynAllocVoice(ALSynth *drvr, ALVoice *voice, ALVoiceConfig *vc)
{
    PVoice *pvoice = 0;
    ALFilter *f;
    ALParam *update;
    s32 stolen;
    voice->priority = vc->priority;
    voice->unityPitch = vc->unityPitch;
    voice->table = 0;
    voice->fxBus = vc->fxBus;
    voice->state = 0;
    voice->pvoice = 0;
    stolen = _allocatePVoice(drvr, &pvoice, vc->priority);
    if (pvoice) {
        f = pvoice->channelKnob;
        if (stolen) {
            pvoice->offset = 512;
            pvoice->vvoice->pvoice = 0;
            update = __allocParam();
            update->delta = drvr->paramSamples;
            update->type = 11;
            update->data.i = 0;
            update->moredata.i = pvoice->offset - 64;
            (*f->setParam)(f, 3, update);
            update = __allocParam();
            if (update) {
                update->delta = drvr->paramSamples + pvoice->offset;
                update->type = 15;
                update->next = 0;
                (*f->setParam)(f, 3, update);
            } else {
            }
        } else {
            pvoice->offset = 0;
        }
        pvoice->vvoice = voice;
        voice->pvoice = pvoice;
    }
    return (pvoice != 0);
}
