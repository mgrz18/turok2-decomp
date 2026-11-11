/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043C688_de_11.c, func_8043CB30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/menu.h"
#include "globals/engine.h"

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
s32 EnterCheatMenu_UpdateTextField(void *unused, MenuRecAt20V4 *menu) {
    f32 t;
    t = gCheatEntry.timer + gFrameIncrement;
    gCheatEntry.timer = t;
    if (t >= 15.0f) {
        gCheatEntry.timer = t - 15.0f;
    }
    if (menu->state == 2) {
        if (T2_MENU_LEFT(menu) != 0 && gCheatEntry.cursor > 0) {
            gCheatEntry.cursor--;
            gCheatEntry.timer = 0.0f;
        } else if (T2_MENU_RIGHT(menu) != 0) {
            if (gCheatEntry.cursor < gCheatEntry.length) {
                gCheatEntry.cursor++;
                gCheatEntry.timer = 0.0f;
            }
        }
    }
    return 0;
}
