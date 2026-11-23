/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_6.c, func_8040A4A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

extern char sDamagedPakMenuText_PakFoundIn[];
extern char sDamagedPakMenuText_DamagedGameCannot[];
s32 DamagedPakMenu_UpdateLine2(struct FieldRecAt14 *field) {
    if (sDamagedPakInGame != 0) {
        field->text = sDamagedPakMenuText_DamagedGameCannot;
    } else {
        field->text = sDamagedPakMenuText_PakFoundIn;
    }
    return 0;
}
