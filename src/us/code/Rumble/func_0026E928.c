/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802846C8_de_17.c, func_802858B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/world.h"
#include "globals/scene.h"

extern void Rumble_Init(NodeCode8028469C *arg0, Clip *arg1, CVector3 arg2, f32 arg5, f32 arg6, void **arg7);
void Rumble_Init(NodeCode8028469C *arg0, Clip *arg1, CVector3 arg2,
                   f32 arg5, f32 arg6, void **arg7) {
    extern void *CScene__GetCurve(void *object, int index);
    extern void RumbleCurve_Init(unsigned int *record, uptr value);
    arg0->key = arg1;
    arg0->position = arg2;
    arg0->value0 = arg5;
    arg0->value1 = arg6;
    arg0->owner = arg7;
    if (arg7 != 0) {
        *arg7 = arg0;
    }
    RumbleCurve_Init(&arg0->amplitudeHandle,
                  (uptr)CScene__GetCurve(&gScene, arg1->mode));
    RumbleCurve_Init(&arg0->lifetimeHandle,
                  (uptr)CScene__GetCurve(&gScene, arg1->frames));
}
