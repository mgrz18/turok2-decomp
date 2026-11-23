/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802636B0_de_9.c, func_8026464C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/controller.h"

s32 Controller_AnyConnected(void) {
    s32 i = 0;
    while (1) {
        if (!(((gContStatus[i].errno >> 3) ^ 1) & 1)) {
            i += 1;
            if (i >= 4) {
                return 0;
            }
        } else {
            return 1;
        }
    }
}
