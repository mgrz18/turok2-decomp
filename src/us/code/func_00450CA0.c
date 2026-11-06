#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/MISC/8043DF84.CPP, func_8043E05C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/controller.h"
#include "structs/list.h"

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
extern ControllerProfile *Menu_FindPressingController_2(s32 *out);
extern s32 Controller_ConfirmPressed(ControllerProfile *);
extern s32 Controller_ConfirmPressed(ControllerProfile *slot);
ControllerProfile *Menu_FindPressingController_2(s32 *out) {
    s32 i;
    ControllerProfile *s;
    i = 0;
    s = D_800F5EC0;
    while (i < 4) {
        if (Controller_ConfirmPressed(s) != 0) {
            *out = i;
            return s;
        }
        i++;
        s++;
    }
    *out = -1;
    return 0;
}
