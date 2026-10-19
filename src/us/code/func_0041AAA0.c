/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043C688_de_11.c, func_8043CB30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int func_00285628(struct ControllerProfile *object);
extern int func_00285670(struct ControllerProfile *object);
extern f32 D_800C1274[];
extern f32 D_800B6D28;
extern f32 D_800C1274[];
s32 func_0041AAA0(void *unused, MenuRecAt20V4 *menu) {
    extern Triple_func_802683E0_de D_800C1F80;
    f32 t;
    t = D_800C1F80.y + D_800B6D28;
    D_800C1F80.y = t;
    if (t >= D_800C1274[1]) {
        D_800C1F80.y = t - D_800C1274[1];
    }
    if (menu->state == 2) {
        if (((s32 (*)(s32))func_00285628)(menu->input) != 0 && D_800C1F80.x > 0) {
            D_800C1F80.x--;
            D_800C1F80.y = 0.0f;
        } else if (((s32 (*)(s32))func_00285670)(menu->input) != 0) {
            if (D_800C1F80.x < D_800C1F80.z) {
                D_800C1F80.x++;
                D_800C1F80.y = 0.0f;
            }
        }
    }
    return 0;
}
