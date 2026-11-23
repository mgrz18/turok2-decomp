/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80265350_de.c, func_80265878_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/cache.h"
#include "globals/boss.h"

#ifdef TARGET_PORT
/* Host body (round 7 v2): the N64 tests the game's data segments (gHashSize + the size symbol
 * D_11059, gPrimagenPaths + D_94F), meaningless in the host link. The only caller
 * (CAI__UpdateModeAnim) asks it of an animation word that is not code: an id (small) or a
 * table in the game data. */
extern int IsCodePointer(uptr arg0);
int IsDataPointer(uptr arg0) {
    return arg0 >= 0x10000 && !IsCodePointer(arg0);
}
#else
extern char D_11059;
extern char D_94F;
int IsDataPointer(uptr arg0) {
    if (arg0 >= (uptr)&gHashSize &&
        arg0 < (uptr)&gHashSize + (uptr)&D_11059) {
        return 1;
    }
    if (arg0 >= (uptr)&gPrimagenPaths &&
        arg0 < (uptr)&gPrimagenPaths + (uptr)&D_94F) {
        return 1;
    }
    return 0;
}
#endif
