#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU_2/80408AC4_7.CPP, func_8043CB30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/controller.h"
#include "structs/list.h"

extern int func_00285628(ControllerProfile *object);
extern int func_00285670(ControllerProfile *object);
struct Shared_Entry;
typedef struct Shared_Entry Shared_Entry;
struct MenuRecAt20V4;
struct MenuRecAt20V4 {
    s16 state;
    char pad2[0x1E];
    s32 input;
};
struct Shared_Entry;
struct Shared_Entry {
    s32 value;
    f32 timer;
    s32 count;
    u8 text[24];
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
extern Shared_Entry D_800C1F80;
extern f32 D_800B6D28;
extern f32 D_800C1274[];
s32 func_00453A08(void *unused, MenuRecAt20V4 *menu) {
    f32 t;
    t = D_800C1F80.timer + D_800B6D28;
    D_800C1F80.timer = t;
    if (t >= D_800C1274[1]) {
        D_800C1F80.timer = t - D_800C1274[1];
    }
    if (menu->state == 2) {
        if (((s32 (*)(s32))func_00285628)(menu->input) != 0 && D_800C1F80.value > 0) {
            D_800C1F80.value--;
            D_800C1F80.timer = 0.0f;
        } else if (((s32 (*)(s32))func_00285670)(menu->input) != 0) {
            if (D_800C1F80.value < D_800C1F80.count) {
                D_800C1F80.value++;
                D_800C1F80.timer = 0.0f;
            }
        }
    }
    return 0;
}
