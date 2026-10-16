/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_8026F620_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
extern float D_800A5410;
void func_0020C618(f32 *out, f32 *left, f32 *right)
{
    out[(0) * 4 + (0)] = (left[(0) * 4] * right[(0)]) + (left[(0) * 4 + 1] * right[4 + (0)]) + (left[(0) * 4 + 2] * right[8 + (0)]);
    out[(0) * 4 + (1)] = (left[(0) * 4] * right[(1)]) + (left[(0) * 4 + 1] * right[4 + (1)]) + (left[(0) * 4 + 2] * right[8 + (1)]);
    out[(0) * 4 + (2)] = (left[(0) * 4] * right[(2)]) + (left[(0) * 4 + 1] * right[4 + (2)]) + (left[(0) * 4 + 2] * right[8 + (2)]);
    out[(1) * 4 + (0)] = (left[(1) * 4] * right[(0)]) + (left[(1) * 4 + 1] * right[4 + (0)]) + (left[(1) * 4 + 2] * right[8 + (0)]);
    out[(1) * 4 + (1)] = (left[(1) * 4] * right[(1)]) + (left[(1) * 4 + 1] * right[4 + (1)]) + (left[(1) * 4 + 2] * right[8 + (1)]);
    out[(1) * 4 + (2)] = (left[(1) * 4] * right[(2)]) + (left[(1) * 4 + 1] * right[4 + (2)]) + (left[(1) * 4 + 2] * right[8 + (2)]);
    out[(2) * 4 + (0)] = (left[(2) * 4] * right[(0)]) + (left[(2) * 4 + 1] * right[4 + (0)]) + (left[(2) * 4 + 2] * right[8 + (0)]);
    out[(2) * 4 + (1)] = (left[(2) * 4] * right[(1)]) + (left[(2) * 4 + 1] * right[4 + (1)]) + (left[(2) * 4 + 2] * right[8 + (1)]);
    out[(2) * 4 + (2)] = (left[(2) * 4] * right[(2)]) + (left[(2) * 4 + 1] * right[4 + (2)]) + (left[(2) * 4 + 2] * right[8 + (2)]);
    out[12] = (left[12] * right[0]) + (left[13] * right[4]) +
              (left[14] * right[8]) + right[12];
    out[13] = (left[12] * right[1]) + (left[13] * right[5]) +
              (left[14] * right[9]) + right[13];
    out[14] = (left[12] * right[2]) + (left[13] * right[6]) +
              (left[14] * right[10]) + right[14];
    out[3] = out[7] = out[11] = 0.0f;
    out[15] = D_800A5410;
}
