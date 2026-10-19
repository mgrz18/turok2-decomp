/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80273D6C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void func_00210C3C(struct Mtx *object) {
    float t;
    t = object->up.x;
    object->up.x = object->forward.x;
    object->forward.x = t;
    t = object->up.y;
    object->up.y = object->forward.y;
    object->forward.y = t;
    t = object->up.z;
    object->up.z = object->forward.z;
    object->forward.z = t;
}
