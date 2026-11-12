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
#ifdef TARGET_PORT
/* the menu by its host layout (round 10 y4): the view's s32 input at N64 0x20 is
 * Menu.controller, a pointer at another host offset, and the call cast it to s32 (macOS, statics
 * above 4 GB: Controller_MenuLeft crashed when a soak run opened the cheat entry; Linux non-PIE
 * statics fit in 32 bits) */
#define T2_MENU_INPUT(m) (((Menu *) (m))->controller)
#define T2_MENU_LEFT(m) Controller_MenuLeft(T2_MENU_INPUT(m))
#define T2_MENU_RIGHT(m) Controller_MenuRight(T2_MENU_INPUT(m))
#else
#define T2_MENU_LEFT(m) ((s32 (*)(s32))Controller_MenuLeft)((m)->input)
#define T2_MENU_RIGHT(m) ((s32 (*)(s32))Controller_MenuRight)((m)->input)
#endif
s32 NameEntryMenu_UpdateName(void *unused, MenuRecAt20V1 *menu) {
    sptr i;
    f32 t;
#ifdef TARGET_PORT
    i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, (uptr) ((Menu *) menu)->player);
#else
    i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, menu->id);
#endif
    t = gNameEntries[i].timer + gFrameIncrement;
    gNameEntries[i].timer = t;
    if (t >= (15.0f)) {
        gNameEntries[i].timer = t - (15.0f);
    }
    if (menu->state == 1) {
        if (T2_MENU_LEFT(menu) != 0 && gNameEntries[i].cursor > 0) {
            gNameEntries[i].cursor--;
            gNameEntries[i].timer = 0.0f;
        } else if (T2_MENU_RIGHT(menu) != 0) {
            if (gNameEntries[i].cursor < gNameEntries[i].length) {
                gNameEntries[i].cursor++;
                gNameEntries[i].timer = 0.0f;
            }
        }
    }
    return 0;
}
