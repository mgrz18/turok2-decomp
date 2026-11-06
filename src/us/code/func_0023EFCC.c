/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AADCC_de_9.c, func_802AC9B8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char sLifeForcePickups;
void *func_0023EFCC(s32 arg0) {
    char *v1;
    s32 i;
    v1 = &sLifeForcePickups;
    i = 1;
    do {
        i -= 1;
        if (((struct EntryRecAtE *)(v1))->id != arg0) {
            v1 += 0x14;
        } else {
            return v1;
        }
    } while (i != -1);
    return 0;
}
