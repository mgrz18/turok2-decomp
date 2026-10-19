/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de_4.c, func_80275AA4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0026BB30(HeaderRecAt1C *dst, Packed *src, char *base, int unused, char *extra);
void func_0026BB30(HeaderRecAt1C *dst, Packed *src, char *base, int unused, char *extra) {
    int i;
    dst->unk0 = src->unk0;
    dst->unk2 = src->unk2;
    dst->unk1C = src->unk10;
    for (i = 0; i < 3; i++) {
        dst->first[i] = base + src->first[i] * 16;
        if (src->second[i] == 0xFFFF) {
            dst->second[i] = 0;
        } else {
            dst->second[i] = extra + src->second[i] * 32;
        }
    }
}
