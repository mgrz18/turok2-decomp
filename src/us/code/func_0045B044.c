#include "globals/menu_desc.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80409A88_9.CPP, func_8040A6DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct RecordRecAt24;
struct RecordRecAt24 {
    char pad[0x1C];
    s32 first;
    s32 second;
    void *resource;
};
struct RecordRecAt24;
extern s32 sGameNoteLoad;
extern s32 GameNoteMenu_SelectNo_2(void *unused, struct RecordRecAt24 *record);
extern char gEngineApp_m_Game_m_MenuPool[];
extern void MenuPool_OpenMenu_2(void *, void *, s32, s32, s32);
s32 GameNoteMenu_SelectNo_2(void *unused, struct RecordRecAt24 *record) {
    do {
        if (sGameNoteLoad != 0) {
            MenuPool_OpenMenu_2(gEngineApp_m_Game_m_MenuPool, &gPakLoadGameMenuDesc_2, record->first, record->second, 0);
        } else {
            MenuPool_OpenMenu_2(gEngineApp_m_Game_m_MenuPool, record->resource, record->first, record->second, 0);
        }
    } while (0);
    return 1;
}
