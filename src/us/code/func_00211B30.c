/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de.c, func_80274B7C_de.
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
extern void func_00211B30(void *arg0, void *arg1, void *arg2);
typedef struct GeometryEdge {
    Vec3 start;
    Vec3 end;
    Vec3 direction;
    Vec3 unitDirection;
    Vec3 normal;
} GeometryEdge;
void func_00211B30(void *arg0, void *arg1, void *arg2) {
    GeometryEdge *dst = arg0;
    Vec3 *a = arg1;
    Vec3 *b = arg2;
    f32 zdiff;
    dst->start = *a;
    dst->end = *b;
    dst->direction.x = b->x - a->x;
    dst->direction.y = b->y - a->y;
    zdiff = b->z - a->z;
    dst->normal.y = 0.0f;
    dst->normal.z = -dst->direction.x;
    dst->direction.z = zdiff;
    dst->normal.x = zdiff;
}
