/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_11.c, func_8024C8C4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0021F9C4(void *arg0, f32 t, void *a, void *b);
void func_0021F9C4(void *arg0, f32 t, void *a, void *b) {
    Vector4f *out = arg0;
    Vector4f *left = a;
    Vector4f *right = b;
    out->x = left->x + (t * (right->x - left->x));
    out->y = left->y + (t * (right->y - left->y));
    out->z = left->z + (t * (right->z - left->z));
    out->w = left->w + (t * (right->w - left->w));
}
