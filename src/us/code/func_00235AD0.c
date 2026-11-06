/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A25D0_de_17.c, func_802A5020_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 Math_AcosLookup(f32 arg0);
extern float func_002974C0(float x);
extern float func_002982F0(float x);
extern float D_800A6508;
extern Vector4f *Quat_AlignYTo(Vector4f *out, u32 bx, u32 by, u32 bz);
extern f32 gQuaternionSin;
extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);
extern f32 func_00298470(f32);
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
Vector4f *Quat_AlignYTo(Vector4f *out, u32 bx, u32 by, u32 bz) {
    CVector3 axis;
    CVector3 cross;
    Vector4f result;
    f32 one;
    f32 magnitude;
    f32 angle;
    f32 trig;
    one = D_800A6508;
    axis.x = 0.0f;
    axis.y = one;
    axis.z = 0.0f;
    CVector3__Cross(&cross, &axis, (CVector3 *)&bx);
    magnitude = func_00298470((cross.x * cross.x) +
                              (cross.y * cross.y) +
                              (cross.z * cross.z));
    if (magnitude == 0.0f) {
        result.z = 0.0f;
        result.y = 0.0f;
        result.x = 0.0f;
        result.w = one;
    } else {
        CVector3__Scale(&cross, &cross, one / magnitude);
        angle = Math_AcosLookup((axis.x * *(f32 *)&bx) +
                              (axis.y * *(f32 *)&by) +
                              (axis.z * *(f32 *)&bz));
        angle *= ((struct func_802077F4_S2 *) ((f32 *) (&D_800A6508)))->unk4;
        trig = func_002982F0(angle);
        result.x = cross.x * trig;
        result.y = cross.y * trig;
        result.z = cross.z * trig;
        gQuaternionSin = trig;
        result.w = func_002974C0(angle);
    }
    *out = result;
    return out;
}
