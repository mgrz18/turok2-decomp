/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80271C54_de_2.c, func_802727D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
struct Matrix;
typedef struct Matrix Matrix;
struct Matrix;
struct Matrix {
    f32 m[16];
};
void func_0020F79C(void *arg0) {
    extern f32 D_800A55E4;
    Matrix *matrix = arg0;
    f32 zero = 0.0f;
    matrix->m[15] = D_800A55E4;
    matrix->m[10] = D_800A55E4;
    matrix->m[5] = D_800A55E4;
    matrix->m[0] = D_800A55E4;
    matrix->m[14] = zero;
    matrix->m[13] = zero;
    matrix->m[12] = zero;
    matrix->m[11] = zero;
    matrix->m[9] = zero;
    matrix->m[8] = zero;
    matrix->m[7] = zero;
    matrix->m[6] = zero;
    matrix->m[4] = zero;
    matrix->m[3] = zero;
    matrix->m[2] = zero;
    matrix->m[1] = zero;
}
