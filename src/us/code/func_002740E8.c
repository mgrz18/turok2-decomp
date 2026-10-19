/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025BDAC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

int func_002740E8(ObjRecAtCV4 *obj, int keyA, int keyB) {
    int i;
    int *keys = &obj->slots[0].keyB;
    for (i = 0; i < 16; i++) {
        if (keys[i * 0x33 - 43] != -1 && obj->owner->local != i && keys[i * 0x33 - 3] == keyA &&
            keys[i * 0x33] == keyB) {
            return i;
        }
    }
    return -1;
}
