/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B53FC_de_padding_B5FFC_12.c, func_802B5540_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py.
 * Typed as libultra's auxbus.c (alAuxBusParam), so the source list is pointer-sized on the host. */

#include "common.h"
#include "structs/libaudio.h"

s32 alAuxBusParam(void *filter, s32 paramID, void *param) {
    ALAuxBus *m = (ALAuxBus *)filter;
    ALFilter **sources = m->sources;

    if (paramID == AL_FILTER_ADD_SOURCE) {
        sources[m->sourceCount++] = (ALFilter *)param;
    }
    return 0;
}
