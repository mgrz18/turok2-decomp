/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80243A90_de_8.c, func_802456A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00284468(s32 unused);
extern void func_00284080();
extern int func_00284174(void);
extern void func_002844F0(void);
extern void func_002052D8(void *arg0, void *arg1);
void func_00284080(void) {
    extern void *D_800C1BB0;
    if (func_00284174() != 0) {
        s32 temp_a1;
        Shared_MenuContext *record;
        func_00284468(1);
        func_002844F0();
        temp_a1 = (s32)((Shared_MenuContext *)D_800C1BB0)->owner;
        if (temp_a1 != 0) {
            func_002052D8(0, temp_a1);
        }
        record = D_800C1BB0;
        record->owner = 0;
        record->resource = 0;
        record->active = 0;
        record->state3C = 0;
        record->unknown60 = 0;
    }
}
int func_00284174(void);
