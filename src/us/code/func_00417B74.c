/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E134_de.c, func_8043DEF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00417B74(void *arg0, s32 arg1);
extern void func_00285A68(s32 arg);
extern void func_00285A80(s32 arg);
void func_00417B74(void *arg0, s32 arg1) {
    s32 val;
    MenuRecAt20V2 *menu = arg0;
    val = menu->widget->unk4;
    if (arg1 != 0) {
        func_00285A68(val);
    } else {
        func_00285A80(val);
    }
}
