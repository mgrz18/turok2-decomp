/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80445964_de_7.c, func_80445964_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int func_00285628(struct ControllerProfile *object);
extern int func_00285670(struct ControllerProfile *object);
extern f32 D_800B6D28;
extern char D_80119870[];
extern Spinner D_800C1FB0[];
extern s32 func_0025340C(char *, s32);
s32 func_0041D2C4(void *unused, MenuRecAt20V1 *menu) {
    s32 i;
    f32 t;
    i = func_0025340C(D_80119870, menu->id);
    t = D_800C1FB0[i].timer + D_800B6D28;
    D_800C1FB0[i].timer = t;
    if (t >= (15.0f)) {
        D_800C1FB0[i].timer = t - (15.0f);
    }
    if (menu->state == 1) {
        if (((s32 (*)(s32))func_00285628)(menu->input) != 0 && D_800C1FB0[i].value > 0) {
            D_800C1FB0[i].value--;
            D_800C1FB0[i].timer = 0.0f;
        } else if (((s32 (*)(s32))func_00285670)(menu->input) != 0) {
            if (D_800C1FB0[i].value < D_800C1FB0[i].max) {
                D_800C1FB0[i].value++;
                D_800C1FB0[i].timer = 0.0f;
            }
        }
    }
    return 0;
}
