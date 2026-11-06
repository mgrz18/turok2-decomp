/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80445964_de_7.c, func_80445964_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/engine.h"

extern int Controller_MenuLeft(struct ControllerProfile *object);
extern int Controller_MenuRight(struct ControllerProfile *object);
extern s32 CPlayerPool__GetIndex(CPlayerPool *, s32);
s32 NameEntryMenu_UpdateName(void *unused, MenuRecAt20V1 *menu) {
    s32 i;
    f32 t;
    i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, menu->id);
    t = gNameEntries[i].timer + gFrameIncrement;
    gNameEntries[i].timer = t;
    if (t >= (15.0f)) {
        gNameEntries[i].timer = t - (15.0f);
    }
    if (menu->state == 1) {
        if (((s32 (*)(s32))Controller_MenuLeft)(menu->input) != 0 && gNameEntries[i].cursor > 0) {
            gNameEntries[i].cursor--;
            gNameEntries[i].timer = 0.0f;
        } else if (((s32 (*)(s32))Controller_MenuRight)(menu->input) != 0) {
            if (gNameEntries[i].cursor < gNameEntries[i].length) {
                gNameEntries[i].cursor++;
                gNameEntries[i].timer = 0.0f;
            }
        }
    }
    return 0;
}
