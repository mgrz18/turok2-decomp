/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_12.c, func_8040AA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int CharNoteMenu_SelectNo(int unused, ItemRecAt24 *item);
extern int D_80130960;
extern char D_80119E2C[];
extern char D_0043B6A8[];
extern void MenuPool_OpenMenu(char *, char *, int, int, int);
int CharNoteMenu_SelectNo(int unused, ItemRecAt24 *item) {
    if (D_80130960 != 0) {
        MenuPool_OpenMenu(D_80119E2C, D_0043B6A8, item->x, item->y, 0);
    } else {
        MenuPool_OpenMenu(D_80119E2C, item->label, item->x, item->y, 0);
    }
    return 1;
}
