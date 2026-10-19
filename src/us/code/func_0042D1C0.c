/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044E8B0.c, func_0044E8B0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void func_0020F040(Vec3f *v);
void func_0042D1C0(S7Light *l) {
    l->dir.x = -0.667424f;
    l->dir.y = 0.953462f;
    l->dir.z = -0.572078f;
    func_0020F040(&l->dir);
    l->enabled = 1;
}
