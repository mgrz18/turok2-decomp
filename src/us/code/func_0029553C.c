/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/alEnvmixerNew.c, alEnvmixerNew.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void alFilterNew(ALFilter *f, void *handler, void *setParam, s32 type);
void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
void *alEnvmixerPull(void);
s32 D_00296118(void *filter, s32 paramID, void *param);
void alEnvmixerNew(ALEnvMixer *e, ALHeap *hp) {
    alFilterNew(&e->filter, alEnvmixerPull, D_00296118, 4);
    e->state = alHeapDBAlloc(0, 0, hp, 1, 0x50);
    e->first = 1;
    e->motion = 0;
    e->volume = 1;
    e->ltgt = 1;
    e->rtgt = 1;
    e->cvolL = 1;
    e->cvolR = 1;
    e->dryamt = 0;
    e->wetamt = 0;
    e->lratm = 1;
    e->lratl = 0;
    *(volatile u16 *) &e->lratl = 0; /* libultra stores lratl twice (where rratl was meant) */
    e->delta = 0;
    e->segEnd = 0;
    e->pan = 0;
    e->ctrlList = 0;
    e->ctrlTail = 0;
    e->sources = 0;
}
