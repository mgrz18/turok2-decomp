/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80255C4C_de_8.c, func_8025623C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

int func_00200A9C(IntrusiveList *arg0, int arg1) {
    int node = arg0->head;
    if (node != 0) {
        do {
            if (node == arg1) {
                return 1;
            }
            node = *(int *)(node + arg0->nextOffset);
        } while (node != 0);
    }
    return 0;
}
