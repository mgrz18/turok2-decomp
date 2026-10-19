/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_8.c, func_8025D158_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s16 func_00293470(void *, void *);
void func_00276D98(struct Shape_typemap_110 *record, s32 resource) {
    extern void *func_002718F4(s32);
    extern char D_800B2AA8[];
    record->field_0 = resource;
    record->field_4 = -1;
    record->field_C = -1;
    record->field_8 = 0;
    record->field_14 = 0;
    record->field_18 = 0x40;
    record->field_1C = func_00293470(func_002718F4(record->field_0), D_800B2AA8);
}
