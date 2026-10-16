/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80233930_de.c, func_80233930_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
extern f32 func_002119FC(f32 arg0, f32 arg1);
extern float func_002982F0(float x);
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
struct StateRecAt10;
typedef struct StateRecAt10 StateRecAt10;
struct StateRecAt10;
struct StateRecAt10 {
    s32 mode;
    f32 value;
    f32 limit;
    f32 rate;
    f32 accumulator;
};
typedef StateRecAt10 AxisWave;
extern float D_800A97E0;
struct Effect33920;
typedef struct Effect33920 Effect33920;
struct Effect33920;
struct Effect33920 {
    s32 unk00;
    Effect33920 *next;
    Vec3 origin;
    f32 radius;
    AxisWave x;
    AxisWave y;
    AxisWave z;
};
extern void func_0027BE90(Effect33920 *arg0, Vec3 position, Vec3 *out);
extern float D_800A97E8;
extern float D_800A97E4;
extern float D_800A97EC;
extern f32 func_00266F50(f32 arg0);
extern void func_0020EF2C(Vec3 *, Vec3 *, Vec3 *);
extern f32 func_00298470(f32);
void func_0027BE90(Effect33920 *arg0, Vec3 position, Vec3 *out) {
    f32 distance;
    f32 value;
    AxisWave *wave;
    func_0020EF2C(&position, &position, &arg0->origin);
    distance = func_00298470((position.x * position.x) +
                             (position.y * position.y) +
                             (position.z * position.z));
    if (distance < arg0->radius) {
        distance = func_00266F50(D_800A97E0 - (distance / arg0->radius));
        wave = &arg0->x;
        if (wave->mode == 0) goto x_sine;
        if (wave->mode != 1) goto x_zero;
        value = func_002982F0(wave->accumulator * D_800A97E4) * wave->rate;
        goto x_done;
x_sine:
        value = func_002119FC(-wave->rate, wave->rate);
        goto x_done;
x_zero:
        value = 0.0f;
x_done:
        out->x += value * distance;
        wave = &arg0->y;
        if (wave->mode == 0) goto y_sine;
        if (wave->mode != 1) goto y_zero;
        value = func_002982F0(wave->accumulator * D_800A97E8) * wave->rate;
        goto y_done;
y_sine:
        value = func_002119FC(-wave->rate, wave->rate);
        goto y_done;
y_zero:
        value = 0.0f;
y_done:
        out->y += value * distance;
        wave = &arg0->z;
        if (wave->mode == 0) goto z_sine;
        if (wave->mode != 1) goto z_zero;
        value = func_002982F0(wave->accumulator * D_800A97EC) * wave->rate;
        goto z_done;
z_sine:
        value = func_002119FC(-wave->rate, wave->rate);
        goto z_done;
z_zero:
        value = 0.0f;
z_done:
        out->z += value * distance;
    }
}
