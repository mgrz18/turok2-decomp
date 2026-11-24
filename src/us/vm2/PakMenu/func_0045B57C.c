/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_12.c, func_8040AA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/pak_2.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"

extern int CharNoteMenu_SelectNo_2(int unused, ItemRecAt24 *item);
int CharNoteMenu_SelectNo_2(int unused, ItemRecAt24 *item) {
    if (sCharNoteLoad != 0) {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakLoadProfileMenuDesc_2, item->x, item->y, 0);
    } else {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, item->label, item->x, item->y, 0);
    }
    return 1;
}
