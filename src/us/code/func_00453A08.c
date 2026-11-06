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
extern f32 D_800C1274[];
extern f32 D_800C1274[];
s32 EnterCheatMenu_UpdateTextField_2(void *unused, MenuRecAt20V4 *menu) {
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
