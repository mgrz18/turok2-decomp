/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_18.CPP, func_8040B3DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/menu.h"
struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};
extern char D_800B437C[];
extern char D_800B436C[];
s32 MissionMenu_UpdateTitle_2(struct FieldRecAt14 *field) {
    if (sMissionFailed != 0) {
        field->text = D_800B437C;
    } else {
        field->text = D_800B436C;
    }
    return 1;
}
