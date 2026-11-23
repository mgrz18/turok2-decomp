/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_11.c, func_8040A9F8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

extern char sCharNoteMenuText_DeleteNote_B4AF8[];
extern char sCharNoteMenuText_NoteInfo[];
extern char sCharNoteMenuText_DeleteNote_B4AEC[];
extern s32 D_801309A8;
s32 CharNoteMenu_UpdateTitle(struct FieldRecAt14 *field) {
    if (sCharNoteLoad != 0) {
        field->text = sCharNoteMenuText_NoteInfo;
    } else if (D_801309A8 != 0) {
        field->text = sCharNoteMenuText_DeleteNote_B4AF8;
    } else {
        field->text = sCharNoteMenuText_DeleteNote_B4AEC;
    }
    return 0;
}
