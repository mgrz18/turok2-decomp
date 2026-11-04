/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80403BCC_8.CPP, func_804053E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/pak.h"

extern s32 func_0045F940(s32 index, s32 entry, s32 *out);
extern s32 func_00426D8C(s32);
s32 func_0045F940(s32 index, s32 entry, s32 *out) {
    if (D_80130C60[index] != 3) {
        return -2;
    }
    *out = func_00426D8C(D_800C2224[index].notes[entry].file_size);
    return 0;
}
