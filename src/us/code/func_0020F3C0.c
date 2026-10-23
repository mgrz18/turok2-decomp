/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80271B18_3.CPP, func_802723FC_de.
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
extern f32 func_00298470(f32);
static __inline__ CVector3 project(CVector3 *arg0, CVector3 *arg1) {
    CVector3 projected;
    f32 dot;
    dot = (arg0->x * arg1->x) + (arg0->y * arg1->y) + (arg0->z * arg1->z);
    projected.x = arg0->x - (dot * arg1->x);
    projected.y = arg0->y - (dot * arg1->y);
    projected.z = arg0->z - (dot * arg1->z);
    return projected;
}
extern f32 func_00298470(f32 arg0);
CVector3 func_0020F3C0(CVector3 *arg0, CVector3 *arg1) {
    CVector3 projected;
    f32 projectedMagnitude;
    f32 magnitudeRatio;
    projected = project(arg0, arg1);
    projectedMagnitude = func_00298470((projected.x * projected.x) +
                                       (projected.y * projected.y) +
                                       (projected.z * projected.z));
    if (projectedMagnitude != 0.0f) {
        magnitudeRatio = func_00298470((arg0->x * arg0->x) +
                                       (arg0->y * arg0->y) +
                                       (arg0->z * arg0->z)) / projectedMagnitude;
        projected.x *= magnitudeRatio;
        projected.y *= magnitudeRatio;
        projected.z *= magnitudeRatio;
    }
    return projected;
}
