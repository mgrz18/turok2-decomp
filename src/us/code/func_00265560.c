/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80203F04_de_3.c, func_80204BD0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/world.h"

extern CScene gScene;
extern s32 CScene__GetInstanceSavedBit(void *, void *);
void Door_Advance(void *arg0, void *arg1) {
    extern s32 CAI__SetMode(void *, void *, s32);
    s32 different = CScene__GetInstanceSavedBit(&gScene, arg0) != 1;
    if (different == 0) {
        CAI__SetMode(arg0, arg1, 0);
    } else {
        CAI__SetMode(arg0, arg1, 1);
    }
}
