/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80233930_de.c, func_80233930_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 Random_Blend(f32 arg0, f32 arg1);
extern float func_002982F0(float x);
extern void CShake__ApplyOffset(Effect33920 *arg0, CVector3 position, CVector3 *out);
extern f32 SmoothStep(f32 arg0);
extern void CVector3__Subtract(CVector3 *, CVector3 *, CVector3 *);
extern f32 func_00298470(f32);
void CShake__ApplyOffset(Effect33920 *arg0, CVector3 position, CVector3 *out) {
    f32 distance;
    f32 value;
    AxisWave *wave;
    CVector3__Subtract(&position, &position, &arg0->origin);
    distance = func_00298470((position.x * position.x) +
                             (position.y * position.y) +
                             (position.z * position.z));
    if (distance < arg0->radius) {
        distance = SmoothStep(1.0f - (distance / arg0->radius));
        wave = &arg0->x;
        if (wave->mode == 0) goto x_sine;
        if (wave->mode != 1) goto x_zero;
        value = func_002982F0(wave->accumulator * 0.017453294f) * wave->rate;
        goto x_done;
x_sine:
        value = Random_Blend(-wave->rate, wave->rate);
        goto x_done;
x_zero:
        value = 0.0f;
x_done:
        out->x += value * distance;
        wave = &arg0->y;
        if (wave->mode == 0) goto y_sine;
        if (wave->mode != 1) goto y_zero;
        value = func_002982F0(wave->accumulator * 0.017453294f) * wave->rate;
        goto y_done;
y_sine:
        value = Random_Blend(-wave->rate, wave->rate);
        goto y_done;
y_zero:
        value = 0.0f;
y_done:
        out->y += value * distance;
        wave = &arg0->z;
        if (wave->mode == 0) goto z_sine;
        if (wave->mode != 1) goto z_zero;
        value = func_002982F0(wave->accumulator * 0.017453294f) * wave->rate;
        goto z_done;
z_sine:
        value = Random_Blend(-wave->rate, wave->rate);
        goto z_done;
z_zero:
        value = 0.0f;
z_done:
        out->z += value * distance;
    }
}
