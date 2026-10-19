/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802636B0_de_9.c, func_802645D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int func_00285A54(int arg0);
extern int D_800F6788[];
s32 func_0028591C(s32 arg0) {
    if (func_00285A54(arg0) == 0) {
        return 0;
    }
    return D_800F6788[arg0];
}
int func_00285A54(int arg0);
