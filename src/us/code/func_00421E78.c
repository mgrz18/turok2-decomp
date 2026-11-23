/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040A500.c, func_0040A500.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "structs/save.h"

extern char sGameNoteMenuText_DifficultyEasy[];
extern char sGameNoteMenuText_DifficultyNormal[];
extern char sGameNoteMenuText_DifficultyHard[];
s32 GameNoteMenu_UpdateDifficulty(MenuItem *item) {
    switch (gLoadBuffer->settings.difficulty) {
    default:
    case 0:
        item->text = sGameNoteMenuText_DifficultyEasy;
        break;
    case 16:
        item->text = sGameNoteMenuText_DifficultyNormal;
        break;
    case 32:
        item->text = sGameNoteMenuText_DifficultyHard;
        break;
    }
    return 0;
}
