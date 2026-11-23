/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_8.c, func_8040A77C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

extern void CharNoteMenu_InitSave_2(struct RecordRecAtCV1 *record);
void CharNoteMenu_InitSave_2(struct RecordRecAtCV1 *record) {
    record->target->first &= ~0x01800000;
    sCharNoteLoad = 0;
    gCharNoteMenuSaving = 1;
    record->target->second &= ~0x01800000;
}
