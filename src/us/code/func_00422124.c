/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_8.c, func_8040A6DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 GameNoteMenu_SelectNo(void *unused, struct RecordRecAt24 *record);
extern s32 D_801309B8;
extern char D_80119E2C[];
extern char D_0043B6F0[];
extern void MenuPool_OpenMenu(void *, void *, s32, s32, s32);
s32 GameNoteMenu_SelectNo(void *unused, struct RecordRecAt24 *record) {
    do {
        if (D_801309B8 != 0) {
            MenuPool_OpenMenu(D_80119E2C, D_0043B6F0, record->first, record->second, 0);
        } else {
            MenuPool_OpenMenu(D_80119E2C, record->resource, record->first, record->second, 0);
        }
    } while (0);
    return 1;
}
