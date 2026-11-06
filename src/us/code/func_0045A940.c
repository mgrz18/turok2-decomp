/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_4.c, func_80409F6C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/world.h"
#include "globals/scene.h"

extern CScene gScene;
extern void CScene__RequestWarp_2(void *, int, int);
extern void Controller_UnwatchPak(void *);
void func_0045A940(void *unused, void *object) {
    Controller_UnwatchPak(object);
    CScene__RequestWarp_2(&gScene, gScene_m_PendingWarpID, 2);
}
