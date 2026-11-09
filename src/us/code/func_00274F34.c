/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_13.c, func_8025D844_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

#ifdef TARGET_PORT
/* The host layout: AudioStateD844 / AudioRootD844 (rw/abu_types.h) are N64-offset views of
 * SndSeq and CAudioWorld (structs/sound.h); the same stores through the real types. */
#include "structs/sound.h"
void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
void alCSPNew(ALCSPlayer *seqp, ALSeqpConfig *config);
void SndSeq_Construct(SndSeq *state, CAudioWorld *root) {
    ALSeqpConfig params;
    ALCSPlayer *player;
    state->owner = root;
    state->unk18 = 16;   /* mode */
    state->track = -1;   /* index */
    state->flags = 0;
    params.maxVoices = 20;
    params.maxEvents = 192;
    params.maxChannels = 20;
    params.heap = &root->heap;
    params.initOsc = 0;
    params.updateOsc = 0;
    params.stopOsc = 0;
    player = alHeapDBAlloc(0, 0, &root->heap, 1, sizeof(ALCSPlayer));
    state->csp = (sptr)player;
    alCSPNew(player, &params);
    state->unk10 = alHeapDBAlloc(0, 0, &root->heap, 1, sizeof(ALCSeq)); /* allocation */
    state->unk24 = 50;    /* timer */
    state->sliding = 0;   /* counter */
    state->unk30 = 1.0f;  /* first */
    state->fadeStep = 125.0f;
    state->current = 1.0f;
    state->target = 1.0f;
}
#else
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
    state->first = 1.0f;
    state->second = 125.0f;
    state->third = 1.0f;
    state->fourth = 1.0f;
}
#endif
