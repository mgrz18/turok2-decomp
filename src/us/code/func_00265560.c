/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80203F04_de_3.c, func_80204BD0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern Shared_World D_800F7078;
extern s32 func_00220408(void *, void *);
void func_00265560(void *arg0, void *arg1) {
    extern s32 func_00243414(void *, void *, s32);
    s32 different = func_00220408(&D_800F7078, arg0) != 1;
    if (different == 0) {
        func_00243414(arg0, arg1, 0);
    } else {
        func_00243414(arg0, arg1, 1);
    }
}
