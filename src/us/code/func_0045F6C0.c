/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_6.c, func_80405160_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int D_80130C60[];
extern int D_80130C70[];
int func_0045F6C0(int slot, int *score) {
    extern int *D_800C2224;
    if (D_80130C60[slot] != 3) {
        return -2;
    }
    if (D_80130C70[slot] == 0) {
        *score = D_800C2224[slot * 0x81] / 256;
    } else {
        *score = 0;
    }
    return D_80130C70[slot];
}
