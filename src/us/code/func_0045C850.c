/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_18.CPP, func_8040B3DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/pak.h"
struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};
extern char D_800B510C[];
extern char D_800B5118[];
s32 DamagedPakMenu_UpdateContinue_2(struct FieldRecAt14 *field) {
    if (gPakMenuIsLoad != 0) {
        field->text = D_800B510C;
    } else {
        field->text = D_800B5118;
    }
    return 0;
}
