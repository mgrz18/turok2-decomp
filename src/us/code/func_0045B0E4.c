/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_8.c, func_8040A77C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0045B0E4(struct RecordRecAtCV1 *record);
extern int D_80130960;
extern s32 D_801309A8;
extern s32 D_80130960;
void func_0045B0E4(struct RecordRecAtCV1 *record) {
    record->target->first &= ~0x01800000;
    D_80130960 = 0;
    D_801309A8 = 1;
    record->target->second &= ~0x01800000;
}
