/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAE40_de_22.c, func_802BB550_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern struct IntPair __osEventStateTab[];
void osSetEventMesg(s32 arg0, s32 arg1, s32 arg2) {
    extern void __osRestoreInt(u32);
    extern u32 __osDisableInt(void);
    u32 temp_v0;
    struct IntPair *e;
    temp_v0 = __osDisableInt();
    ;
    (&__osEventStateTab[arg0])->field_0 = arg1;
    (*(&__osEventStateTab[arg0])).field_4 = arg2;
    __osRestoreInt(temp_v0);
}
