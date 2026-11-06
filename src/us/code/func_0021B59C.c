/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80246E44_de_7.c, func_802480F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern SineTable sWobbleSineTable;
extern s32 gFrameCount;
extern void CVector3__Normalize(CVector3 *);
extern void CMtxF__Copy(f32 *, f32 *);
extern void CVector3__Scale(CVector3 *, CVector3 *, f32);
extern void CMtxF__Translate(f32 *, f32, f32, f32);
extern void CMtxF__SetAxisAngle(f32 *, f32, f32, f32, f32);
extern void CMtxF__MultiplyAffine(f32 *, f32 *, f32 *);
void func_0021B59C(s32 time, s32 flags, f32 *matrix) {
    f32 rotation[16];
    f32 inverse[16];
    CVector3 origin;
    CVector3 axis;
    u32 t;
    f32 angle;
    f32 flip;
    s32 mode;
    t = gFrameCount;
    t += (flags & 0x38) * 50;
    t += (u32)(time & 0xFFFF) / 113;
    if (!(flags & 4)) {
        t += 40;
    }
    axis.x = 0.0f;
    axis.z = 0.0f;
    axis.y = 1.0f;
    mode = flags & 0xC3;
    if (mode == 2) {
        axis.z = 1.0f;
        axis.x = 0.0f;
        angle = sWobbleSineTable.value[t % 120] - 0.5f;
        axis.y = sWobbleSineTable.value[(t + 12) % 120];
    } else if (mode == 0x82) {
        axis.x = 1.0f;
        axis.z = 0.0f;
        angle = sWobbleSineTable.value[t % 120] - 0.5f;
        axis.y = sWobbleSineTable.value[(t + 12) % 120];
        angle *= 2.0f;
    } else if (mode == 0x40) {
        angle = sWobbleSineTable.value[(t + 30) % 120] - 0.5f;
        axis.x = sWobbleSineTable.value[(t + 12) % 120];
        axis.y = sWobbleSineTable.value[(t + 42) % 120];
        axis.z = sWobbleSineTable.value[(t + 110) % 120];
        angle *= 2.0f;
    } else if (mode == 1) {
        angle = sWobbleSineTable.value[(t + 70) % 120] - 0.5f;
        angle *= 0.7f;
    } else {
        return;
    }
    CVector3__Normalize(&axis);
    origin.x = matrix[12];
    origin.y = matrix[13];
    origin.z = matrix[14];
    CMtxF__Copy(inverse, matrix);
    flip = -1.0f;
    CVector3__Scale(&origin, &origin, flip);
    CMtxF__Translate(inverse, origin.x, origin.y, origin.z);
    CMtxF__SetAxisAngle(rotation, angle, axis.x, axis.y, axis.z);
    CMtxF__MultiplyAffine(matrix, inverse, rotation);
    CVector3__Scale(&origin, &origin, flip);
    CMtxF__Translate(matrix, origin.x, origin.y, origin.z);
}
