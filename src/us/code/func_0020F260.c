/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80271B18_2.CPP, func_8027229C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
struct CVector3_t;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
f32 func_00298470(f32);
extern float D_800A55D4;
extern CVector3 *func_0020F260(CVector3 *out, CVector3 *value, CVector3 *a, CVector3 *b);
static __inline__ void normalizeProjectionNormal(CVector3 *normal) {
    f32 length;
    f32 inverse;
    length = func_00298470(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
    if (length != 0.0f) {
        inverse = D_800A55D4 / length;
        normal->x *= inverse;
        normal->y *= inverse;
        normal->z *= inverse;
    }
}
static __inline__ void projectionCross(CVector3 *normal, CVector3 *a, CVector3 *b) {
    normal->x = a->y * b->z - a->z * b->y;
    normal->y = a->z * b->x - a->x * b->z;
    normal->z = a->x * b->y - a->y * b->x;
}
static __inline__ void projectionScale(CVector3 *out, CVector3 *normal, f32 projection) {
    out->x = normal->x * projection;
    out->y = normal->y * projection;
    out->z = normal->z * projection;
}
CVector3 *func_0020F260(CVector3 *out, CVector3 *value, CVector3 *a, CVector3 *b) {
    CVector3 normal;
    CVector3 projected;
    f32 length;
    f32 projection;
    projectionCross(&normal, a, b);
    normalizeProjectionNormal(&normal);
    projection = value->x * normal.x + value->y * normal.y + value->z * normal.z;
    projectionScale(&projected, &normal, projection);
    *out = projected;
    return out;
}
