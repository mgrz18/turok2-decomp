/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_18.CPP, func_8040B3DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/pak.h"
struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};
extern char D_800B4B84[];
extern char D_800B4B8C[];
s32 GameNoteMenu_UpdatePrompt_2(struct FieldRecAt14 *field) {
    if (sGameNoteLoad != 0) {
        field->text = D_800B4B84;
    } else {
        field->text = D_800B4B8C;
    }
    return 0;
}
