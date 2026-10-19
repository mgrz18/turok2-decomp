/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802846C8_de_17.c, func_802858B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern Shared_World D_800F7078;
extern void func_0026E928(NodeCode8028469C *arg0, Clip *arg1, Vec3 arg2, f32 arg5, f32 arg6, void **arg7);
void func_0026E928(NodeCode8028469C *arg0, Clip *arg1, Vec3 arg2,
                   f32 arg5, f32 arg6, void **arg7) {
    extern void *func_00225D94(void *object, int index);
    extern void func_0026EE30(unsigned int *record, unsigned int value);
    arg0->key = arg1;
    arg0->position = arg2;
    arg0->value0 = arg5;
    arg0->value1 = arg6;
    arg0->owner = arg7;
    if (arg7 != 0) {
        *arg7 = arg0;
    }
    func_0026EE30(&arg0->amplitudeHandle,
                  (unsigned int)func_00225D94(&D_800F7078, arg1->mode));
    func_0026EE30(&arg0->lifetimeHandle,
                  (unsigned int)func_00225D94(&D_800F7078, arg1->frames));
}
