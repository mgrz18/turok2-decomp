/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E134_de.c, func_8043DEF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00417B74(void *arg0, s32 arg1);
extern void Controller_WatchPak(s32 arg);
extern void Controller_UnwatchPak(sptr arg);
void func_00417B74(void *arg0, s32 arg1) {
    s32 val;
    MenuRecAt20V2 *menu = arg0;
    val = menu->widget->unk4;
    if (arg1 != 0) {
        Controller_WatchPak(val);
    } else {
        Controller_UnwatchPak(val);
    }
}
