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
struct Clip;
struct Clip {
    s16 mode;
    s16 frames;
};
struct Clip;
typedef struct Clip Clip;
struct NodeCode8028469C;
struct NodeCode8028469C {
    struct NodeCode8028469C *previous;
    struct NodeCode8028469C *next;
    Clip *key;
    CVector3 position;
    f32 value0;
    f32 value1;
    u32 amplitudeHandle;
    char pad24[8];
    u32 lifetimeHandle;
    char pad30[8];
    struct NodeCode8028469C **owner;
};
struct PoolRecAt28V1 {
    CList free;
    CList active;
    s32 baseLevel;
};
struct NodeCode8028469C;
typedef struct NodeCode8028469C NodeCode8028469C;
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
