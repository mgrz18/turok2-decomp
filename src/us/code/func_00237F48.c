/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de.c, func_80267198_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00237F48(void *actor, void *source, s32 event, Vec3 position, struct ResourceManagerState state);
void func_00237F48(void *arg0, void *arg1, s32 arg2, Vec3 arg3, ResourceManagerState arg6) {
    extern CallbackEntryRecAt4V1 D_800AD080[];
    if (D_800AD080[arg2].callback != 0) {
        D_800AD080[arg2].callback(arg0, arg1, arg2, arg3, arg6);
    }
}
