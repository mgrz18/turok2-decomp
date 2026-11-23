/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_18.CPP, func_8040B3DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/pak.h"
struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};
extern char sGameNoteMenuText_NoteInfo[];
extern char sGameNoteMenuText_DeleteNote[];
s32 GameNoteMenu_UpdateTitle_2(struct FieldRecAt14 *field) {
    if (sGameNoteLoad != 0) {
        field->text = sGameNoteMenuText_NoteInfo;
    } else {
        field->text = sGameNoteMenuText_DeleteNote;
    }
    return 0;
}
