/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80271C54_de_2.c, func_80272CB0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
struct Matrix;
typedef struct Matrix Matrix;
struct Matrix;
struct Matrix {
    f32 m[16];
};
extern void func_0020FC74(void *matrix, f32 x, f32 y, f32 z);
void func_0020FC74(void *arg0, f32 arg1, f32 arg2, f32 arg3) {
    extern f32 D_800A55F4[2];
    Matrix *o = arg0;
    f32 val = D_800A55F4[1];
    f32 zero = 0.0f;
    o->m[0] = arg1;
    o->m[5] = arg2;
    o->m[10] = arg3;
    o->m[15] = val;
    o->m[14] = zero;
    o->m[13] = zero;
    o->m[12] = zero;
    o->m[11] = zero;
    o->m[9] = zero;
    o->m[8] = zero;
    o->m[7] = zero;
    o->m[6] = zero;
    o->m[4] = zero;
    o->m[3] = zero;
    o->m[2] = zero;
    o->m[1] = zero;
}
