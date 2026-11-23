#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/MISC/8043DF84.CPP, func_8043E05C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/controller.h"
#include "structs/list.h"
extern ControllerProfile *Menu_FindPressingController_2(s32 *out);
extern s32 Controller_ConfirmPressed(ControllerProfile *);
extern s32 Controller_ConfirmPressed(ControllerProfile *slot);
ControllerProfile *Menu_FindPressingController_2(s32 *out) {
    s32 i;
    ControllerProfile *s;
    i = 0;
    s = gControllers;
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
