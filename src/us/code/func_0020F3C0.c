#include "structs/vector.h"
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck struct_scalar): the callers pass a result pointer first, as
 * MIPS does for a struct returned by value; x86-64 and arm64 return this struct in registers.
 * The definition keeps its by-value form as CVector3__ProjectKeepLength_byval; CVector3__ProjectKeepLength below takes the pointer. */
#define CVector3__ProjectKeepLength CVector3__ProjectKeepLength_byval
#endif

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80271B18_3.CPP, func_802723FC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

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
CVector3 CVector3__ProjectKeepLength(CVector3 *arg0, CVector3 *arg1) {
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

#ifdef TARGET_PORT
#undef CVector3__ProjectKeepLength
void CVector3__ProjectKeepLength(CVector3 *out, CVector3 *arg0, CVector3 *arg1) {
    *out = CVector3__ProjectKeepLength_byval(arg0, arg1);
}
#endif
