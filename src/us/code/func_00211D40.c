/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de.c, func_80274D8C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
extern s32 func_00211D40(void *arg0, void *arg1);
typedef struct GeometryEdge {
    Vec3 start;
    Vec3 end;
    Vec3 direction;
    Vec3 unitDirection;
    Vec3 normal;
} GeometryEdge;
s32 func_00211D40(void *arg0, void *arg1) {
    GeometryEdge *edge = arg0;
    Vec3 *point = arg1;
    Vec3 normal;
    Vec3 delta;
    s32 result;
    normal = edge->normal;
    delta.x = point->x - edge->start.x;
    delta.z = point->z - edge->start.z;
    result = 0;
    if (!(((normal.x * delta.x) + (normal.z * delta.z)) <= 0.0f)) {
        result = 1;
    }
    return result;
}
