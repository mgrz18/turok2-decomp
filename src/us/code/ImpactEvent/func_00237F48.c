/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de.c, func_80267198_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/particle.h"

extern void ImpactEvent_Call2(void *actor, void *source, s32 event, CVector3 position, struct ResourceManagerState state);
void ImpactEvent_Call2(void *arg0, void *arg1, s32 arg2, CVector3 arg3, ResourceManagerState arg6) {
    if (gImpactEventHandlers[arg2].handler != 0) {
        ((func_80267198_de_Callback) gImpactEventHandlers[arg2].handler)(arg0, arg1, arg2, arg3, arg6);
    }
}
