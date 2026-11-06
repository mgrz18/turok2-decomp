/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043C688_de_11.c, func_8043CB30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/menu.h"

extern int Controller_MenuLeft(struct ControllerProfile *object);
extern int Controller_MenuRight(struct ControllerProfile *object);
extern f32 D_800C1274[];
extern f32 gFrameIncrement;
extern f32 D_800C1274[];
s32 EnterCheatMenu_UpdateTextField(void *unused, MenuRecAt20V4 *menu) {
    f32 t;
    t = gCheatEntry.timer + gFrameIncrement;
    gCheatEntry.timer = t;
    if (t >= D_800C1274[1]) {
        gCheatEntry.timer = t - D_800C1274[1];
    }
    if (menu->state == 2) {
        if (((s32 (*)(s32))Controller_MenuLeft)(menu->input) != 0 && gCheatEntry.cursor > 0) {
            gCheatEntry.cursor--;
            gCheatEntry.timer = 0.0f;
        } else if (((s32 (*)(s32))Controller_MenuRight)(menu->input) != 0) {
            if (gCheatEntry.cursor < gCheatEntry.length) {
                gCheatEntry.cursor++;
                gCheatEntry.timer = 0.0f;
            }
        }
    }
    return 0;
}
