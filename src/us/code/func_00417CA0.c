/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E134_de_2.c, func_8043E05C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/controller.h"

extern ControllerProfile *Menu_FindPressingController(s32 *out);
extern s32 Controller_ConfirmPressed(ControllerProfile *);
extern s32 Controller_ConfirmPressed(ControllerProfile *slot);
ControllerProfile *Menu_FindPressingController(s32 *out) {
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
