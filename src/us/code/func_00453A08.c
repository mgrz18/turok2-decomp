#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU_2/80408AC4_7.CPP, func_8043CB30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/controller.h"
#include "structs/list.h"
#include "structs/menu.h"
#include "globals/engine.h"

extern int Controller_MenuLeft(ControllerProfile *object);
extern int Controller_MenuRight(ControllerProfile *object);
struct MenuRecAt20V4;
struct MenuRecAt20V4 {
    s16 state;
    char pad2[0x1E];
    s32 input;
};
struct MenuRecAt20V4;
typedef struct MenuRecAt20V4 MenuRecAt20V4;
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
s32 EnterCheatMenu_UpdateTextField_2(void *unused, MenuRecAt20V4 *menu) {
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
