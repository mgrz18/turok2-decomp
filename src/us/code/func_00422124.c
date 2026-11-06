/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_8.c, func_8040A6DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"

extern s32 GameNoteMenu_SelectNo(void *unused, struct RecordRecAt24 *record);
extern void MenuPool_OpenMenu(void *, void *, s32, s32, s32);
s32 GameNoteMenu_SelectNo(void *unused, struct RecordRecAt24 *record) {
    do {
        if (sGameNoteLoad != 0) {
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakLoadGameMenuDesc, record->first, record->second, 0);
        } else {
            MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, record->resource, record->first, record->second, 0);
        }
    } while (0);
    return 1;
}
