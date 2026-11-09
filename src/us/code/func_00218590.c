/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802609AC_de_2.c, func_802610F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0029E460(char *data, char *source, s32 size); /* memcpy */
/* Appends a block of size bytes to the pack; copies prepare (a source address) into it when given.
 * m2c dropped the copy's source and size (still in a1/a2 on the N64): on the host nothing was copied. */
char *Pack_Add(Pack *pack, sptr prepare, s32 size)
{
    s32 i;
    char *data;
    for (i = 0; i <= pack->count; i++) {
        if (pack->offsets[i] == 0xDEADBEEF) {
            break;
        }
    }
    i--;
    pack->offsets[i + 1] = pack->offsets[i] + size;
    data = (char *)pack + pack->offsets[i];
    if (prepare) {
        func_0029E460(data, (char *)prepare, size);
    }
    return data;
}
