/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802846C8_de_17.c, func_802859D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned int u32;
typedef float f32;
extern float D_800A8058;
extern float D_800A8060;
extern f32 func_0026EA48(f32 *arg0, u32 arg1, u32 arg2, u32 arg3);
extern f32 func_00298470(f32);
extern f32 func_0026EF30(void *arg0);
f32 func_0026EA48(f32 *arg0, u32 arg1, u32 arg2, u32 arg3) {
    f32 magnitude;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 limit;
    f32 factor;
    f32 first;
    dx = *(f32 *)&arg1 - arg0[3];
    dy = *(f32 *)&arg2 - arg0[4];
    dz = *(f32 *)&arg3 - arg0[5];
    magnitude = func_00298470((dx * dx) + (dy * dy) + (dz * dz));
    if ((magnitude == 0.0f) || ((limit = arg0[6]) == 0.0f)) {
        factor = *(&D_800A8058 + 1);
    } else {
        factor = 0.0f;
        if (magnitude < limit) {
            factor = D_800A8060 - (magnitude / limit);
        }
    }
    factor *= arg0[7];
    first = func_0026EF30(&arg0[8]);
    return factor * first * func_0026EF30(&arg0[11]);
}
