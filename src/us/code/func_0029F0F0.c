/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/xlitob.c, lldiv.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void func_0029F0F0(lldiv_t *ans, s64 num, s64 den);
void func_0029F0F0(lldiv_t *ans, s64 num, s64 den) {
    ans->quot = num / den;
    ans->rem = num % den;
}
