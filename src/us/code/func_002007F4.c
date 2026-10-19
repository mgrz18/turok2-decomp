/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80255C4C_de_8.c, func_80255F94_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002007F4(IntrusiveList *arg0);
void func_002007F4(IntrusiveList *arg0) {
    char *cursor = arg0->tail;
    int stride = arg0->prevOffset;
    int value = *(int *)(cursor + stride);
    int count = arg0->count - 1;
    arg0->count = count;
    arg0->tail = value;
}
