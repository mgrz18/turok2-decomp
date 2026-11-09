#include "common.h"
#include "structs/libaudio.h"

/* alSynAllocFX (libultra synallocfx.c): the effect of aux bus `bus` (s->auxBus[bus].fx[0],
 * 0x20 into each 0x4C-byte ALAuxBus) is created, fed from the bus and added to the main bus. */
void alFxNew(ALFx *r, ALSynConfig *c, ALHeap *hp);
s32 alFxParam(void *filter, s32 paramID, void *param);
s32 alMainBusParam(void *filter, s32 paramID, void *param);

ALFxRef alSynAllocFX(ALSynth *s, s16 bus, ALSynConfig *c, ALHeap *hp) {
    alFxNew(&s->auxBus[bus].fx[0], c, hp);
    alFxParam(&s->auxBus[bus].fx[0], AL_FILTER_SET_SOURCE, &s->auxBus[bus]);
    alMainBusParam(s->mainBus, AL_FILTER_ADD_SOURCE, &s->auxBus[bus].fx[0]);
    return (ALFxRef)&s->auxBus[bus].fx[0];
}
