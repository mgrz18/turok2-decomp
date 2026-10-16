/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_10.c, func_8024C874_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
extern void func_0020EF8C(void *arg0, f32 t, void *a, void *b);
void func_0020EF8C(void *arg0, f32 t, void *a, void *b) {
    Vec3 *out = arg0;
    Vec3 *start = a;
    Vec3 *end = b;
    out->x = start->x + (t * (end->x - start->x));
    out->y = start->y + (t * (end->y - start->y));
    out->z = start->z + (t * (end->z - start->z));
}
