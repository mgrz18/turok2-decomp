/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043C688_de_11.c, func_8043CB30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/menu.h"
#include "globals/engine.h"

extern int Controller_MenuLeft(struct ControllerProfile *object);
extern int Controller_MenuRight(struct ControllerProfile *object);
s32 EnterCheatMenu_UpdateTextField(void *unused, MenuRecAt20V4 *menu) {
    f32 t;
#ifdef TARGET_PORT
    /* The window on the host (STABILITY.md 7.6, the PIE soak): the Rage Wars view read the
     * controller as an s32 at 0x20 (on the host Menu::desc) and called through an (s32) cast; a
     * PIE puts &gControllers above 4 GB and the call got a truncated pointer. */
    Menu *win = (Menu *) menu;
#endif
    t = gCheatEntry.timer + gFrameIncrement;
    gCheatEntry.timer = t;
    if (t >= 15.0f) {
        gCheatEntry.timer = t - 15.0f;
    }
#ifdef TARGET_PORT
    if (win->cursor == 2) {
        if (Controller_MenuLeft(win->controller) != 0 && gCheatEntry.cursor > 0) {
#else
    if (menu->state == 2) {
        if (((s32 (*)(s32))Controller_MenuLeft)(menu->input) != 0 && gCheatEntry.cursor > 0) {
#endif
            gCheatEntry.cursor--;
            gCheatEntry.timer = 0.0f;
#ifdef TARGET_PORT
        } else if (Controller_MenuRight(win->controller) != 0) {
#else
        } else if (((s32 (*)(s32))Controller_MenuRight)(menu->input) != 0) {
#endif
            if (gCheatEntry.cursor < gCheatEntry.length) {
                gCheatEntry.cursor++;
                gCheatEntry.timer = 0.0f;
            }
        }
    }
    return 0;
}
