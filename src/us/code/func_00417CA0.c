/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E134_de_2.c, func_8043E05C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern ControllerProfile *func_00417CA0(s32 *out);
extern s32 func_002855E8(ControllerProfile *);
extern s32 func_002855E8(ControllerProfile *slot);
ControllerProfile *func_00417CA0(s32 *out) {
    extern ControllerProfile D_800F5EC0[4];
    s32 i;
    ControllerProfile *s;
    i = 0;
    s = D_800F5EC0;
    while (i < 4) {
        if (func_002855E8(s) != 0) {
            *out = i;
            return s;
        }
        i++;
        s++;
    }
    *out = -1;
    return 0;
}
