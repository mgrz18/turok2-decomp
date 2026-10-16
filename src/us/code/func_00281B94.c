/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80233930_de_30.c, func_80239D1C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
extern f32 func_002119FC(f32 arg0, f32 arg1);
extern float func_002982F0(float x);
struct Wave;
struct Wave {
    s32 kind;
    char pad4[0xC - 4];
    f32 amplitude;
    f32 phase;
};
extern float D_800A9C2C;
struct Wave;
extern f32 func_00281B94(struct Wave *wave);
f32 func_00281B94(struct Wave *wave) {
    switch (wave->kind) {
    case 1:
        return func_002982F0(wave->phase * D_800A9C2C) * wave->amplitude;
    case 0:
        return func_002119FC(-wave->amplitude, wave->amplitude);
    }
    return 0.0f;
}
