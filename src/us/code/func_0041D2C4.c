/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80445964_de_7.c, func_80445964_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "protos/player.h"

extern int Controller_MenuLeft(struct ControllerProfile *object);
extern int Controller_MenuRight(struct ControllerProfile *object);
s32 NameEntryMenu_UpdateName(void *unused, MenuRecAt20V1 *menu) {
    sptr i;
    f32 t;
#ifdef TARGET_PORT
    /* The window on the host (STABILITY.md 7.6, the PIE soak): the Rage Wars view read the
     * controller as an s32 at 0x20 (on the host Menu::desc) and called through an (s32) cast; a
     * PIE puts &gControllers above 4 GB and the call got a truncated pointer. */
    Menu *win = (Menu *) menu;
#endif
#ifdef TARGET_PORT
    i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, (uptr) win->player);
#else
    i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, menu->id);
#endif
    t = gNameEntries[i].timer + gFrameIncrement;
    gNameEntries[i].timer = t;
    if (t >= (15.0f)) {
        gNameEntries[i].timer = t - (15.0f);
    }
#ifdef TARGET_PORT
    if (win->cursor == 1) {
        if (Controller_MenuLeft(win->controller) != 0 && gNameEntries[i].cursor > 0) {
#else
    if (menu->state == 1) {
        if (((s32 (*)(s32))Controller_MenuLeft)(menu->input) != 0 && gNameEntries[i].cursor > 0) {
#endif
            gNameEntries[i].cursor--;
            gNameEntries[i].timer = 0.0f;
#ifdef TARGET_PORT
        } else if (Controller_MenuRight(win->controller) != 0) {
#else
        } else if (((s32 (*)(s32))Controller_MenuRight)(menu->input) != 0) {
#endif
            if (gNameEntries[i].cursor < gNameEntries[i].length) {
                gNameEntries[i].cursor++;
                gNameEntries[i].timer = 0.0f;
            }
        }
    }
    return 0;
}
