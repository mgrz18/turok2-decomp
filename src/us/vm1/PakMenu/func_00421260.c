/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00409824.c, func_00409824.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "globals/pak.h"
s32 Str_Length(char *s);
s32 Str_Compare(char *a, char *b);
s32 Pak_IsTurok2Note(char *a, char *b) {
    if (Str_Length(b) == 4 && Str_Length(a) == 2 && Str_Compare(b, gPakGameCode) == 0 && Str_Compare(a, gPakCompanyCode) == 0) {
        return 1;
    }
    return 0;
}
