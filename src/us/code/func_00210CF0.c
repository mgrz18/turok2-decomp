/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80273E20_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00210CF0(struct Mtx *object);
void func_00210CF0(struct Mtx *object) {
    float t;
    t = object->right.y;
    object->right.y = object->right.z;
    object->right.z = t;
    t = object->up.y;
    object->up.y = object->up.z;
    object->up.z = t;
    t = object->forward.y;
    object->forward.y = object->forward.z;
    object->forward.z = t;
    t = object->m31;
    object->m31 = object->m32;
    object->m32 = t;
}
