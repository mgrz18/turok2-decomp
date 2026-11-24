/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_8.c, func_8040A6DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/pak_2.h"
#include "globals/menu.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"

#ifdef TARGET_PORT
/* The port: RecordRecAt24 reads the player, the controller and the context at N64 offsets 0x1C/0x20/0x24;
 * the host Menu (structs/menu.h) has them pointer-wide further on. */
s32 GameNoteMenu_SelectNo_2(void *unused, Menu *record) {
    if (sGameNoteLoad != 0) {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, &gPakLoadGameMenuDesc_2, (sptr) record->player,
                          (sptr) record->controller, 0);
    } else {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, record->context, (sptr) record->player,
                          (sptr) record->controller, 0);
    }
    return 1;
}
#else
/* The port body's signature; the N64 body reads the record through the Rage Wars view (a macro). */
#define record ((struct RecordRecAt24 *) recordArg)
s32 GameNoteMenu_SelectNo_2(void *unused, Menu *recordArg) {
    do {
        if (sGameNoteLoad != 0) {
            MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, &gPakLoadGameMenuDesc_2, record->first, record->second, 0);
        } else {
            MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, record->resource, record->first, record->second, 0);
        }
    } while (0);
    return 1;
}
#undef record
#endif
