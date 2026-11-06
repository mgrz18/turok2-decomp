/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80265350_de.c, func_80265878_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char gHashSize;
extern char D_11059;
extern char gPrimagenPaths;
extern char D_94F;
int IsDataPointer(u32 arg0) {
    if (arg0 >= (u32)&gHashSize &&
        arg0 < (u32)&gHashSize + (u32)&D_11059) {
        return 1;
    }
    if (arg0 >= (u32)&gPrimagenPaths &&
        arg0 < (u32)&gPrimagenPaths + (u32)&D_94F) {
        return 1;
    }
    return 0;
}
