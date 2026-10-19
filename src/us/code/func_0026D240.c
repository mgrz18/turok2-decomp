/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80279198_de_8.c, func_802794C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void func_0026D240(void *arg0, void *arg1) {
    if ((((struct ListHeader *) ((s8 *) arg0))->count) == 0) {
        (((struct ListHeader *) ((s8 *) arg0))->head) = arg1;
        (((struct ListHeader *) ((s8 *) arg0))->tail) = arg1;
        (((struct Shape_typemap_3 *) ((s8 *) arg1))->field_0) = 0;
        (((struct func_80203E78_S1 *) ((s8 *) arg1))->unk4) = 0;
    } else {
        (((struct Field_void_4 *) ((s8 *) arg1))->value) = (((struct ListHeader *) ((s8 *) arg0))->head);
        (((struct Shape_typemap_3 *) ((s8 *) arg1))->field_0) = 0;
        (((struct func_80284AF4_G2 *) ((s8 *) ((struct ListHeader *) ((s8 *) arg0))->head))->unk0) = arg1;
        (((struct ListHeader *) ((s8 *) arg0))->head) = arg1;
    }
    (((struct ListHeader *) ((s8 *) arg0))->count) = (((struct ListHeader *) ((s8 *) arg0))->count) + 1;
}
