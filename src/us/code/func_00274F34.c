/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_13.c, func_8025D844_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800A81C8;
extern float D_800A81CC;
s32 alHeapDBAlloc(s32 a, s32 b, void *c, s32 d, s32 e);
void SndSeq_Construct(struct AudioStateD844 *state, struct AudioRootD844 *root) {
    ALSeqpConfig params;
    void *player;
    state->root = root;
    state->mode = 16;
    state->index = -1;
    state->flags = 0;
    params.maxVoices = 20;
    params.maxEvents = 192;
    params.maxChannels = 20;
    params.heap = &state->root->heap;
    params.initOsc = 0;
    params.updateOsc = 0;
    params.stopOsc = 0;
    player = alHeapDBAlloc(0, 0, &state->root->heap, 1, 0x7C);
    state->player = player;
    alCSPNew(player, &params);
    state->allocation = alHeapDBAlloc(0, 0, &state->root->heap, 1, 0xF8);
    state->timer = 50;
    state->counter = 0;
    state->first = D_800A81C8;
    state->second = D_800A81CC;
    state->third = D_800A81C8;
    state->fourth = D_800A81C8;
}
