/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de.c, func_80254E30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void *func_00206658(void) {
    extern void CList__Remove(void *, s32);
    extern s32 CList__AddHead(void *, s32);
    extern s32 D_800D8D68;
    void **p = &D_800D8D68;
    void *temp_s0 = *p;
    if (temp_s0 != 0) {
        CList__Remove(p, (s32) temp_s0);
        ((ResourceLoadRequest *)temp_s0)->flags = 1;
        CList__AddHead(((char *)p + sizeof(IntrusiveList)), (s32) temp_s0);
    }
    return temp_s0;
}
