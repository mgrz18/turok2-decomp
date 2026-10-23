/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RSP/8026E1F8_5.CPP, func_8026EF58_de, with Turok 2's layouts. */
#include "common.h"

/*
 * Invert a row-major affine 4x4 transform (last column 0,0,0,1, translation in
 * the last row); a singular input is copied to out unchanged.
 */
void func_0020C208(f32 *out, f32 *m) {
    f32 determinant;
    f32 reciprocalDeterminant;
    f32 m01TimesM22;
    f32 m00TimesM12;
    f32 inverseTranslationZNumeratorPartial;
    f32 one;

    determinant = m[0] * m[5] * m[10]
                - m[0] * m[9] * m[6]
                - m[4] * m[1] * m[10]
                + m[4] * m[9] * m[2]
                + m[8] * m[1] * m[6]
                - m[8] * m[5] * m[2];

    if (determinant == 0.0f) {
        out[0] = m[0];
        out[1] = m[1];
        out[2] = m[2];
        out[3] = m[3];
        out[4] = m[4];
        out[5] = m[5];
        out[6] = m[6];
        out[7] = m[7];
        out[8] = m[8];
        out[9] = m[9];
        out[10] = m[10];
        out[11] = m[11];
        out[12] = m[12];
        out[13] = m[13];
        out[14] = m[14];
        out[15] = m[15];
        return;
    }

    one = 1.0f;
    reciprocalDeterminant = one / determinant;
    out[0] = (m[5] * m[10] - m[9] * m[6]) * reciprocalDeterminant;
    m01TimesM22 = m[1] * m[10];
    out[1] = (m[9] * m[2] - m01TimesM22) * reciprocalDeterminant;
    out[2] = (m[1] * m[6] - m[5] * m[2]) * reciprocalDeterminant;
    out[3] = 0.0f;
    out[4] = (-m[4] * m[10] + m[8] * m[6]) * reciprocalDeterminant;
    out[5] = (m[0] * m[10] - m[8] * m[2]) * reciprocalDeterminant;
    m00TimesM12 = m[0] * m[6];
    out[6] = (m[4] * m[2] - m00TimesM12) * reciprocalDeterminant;
    out[7] = 0.0f;
    out[8] = -(-m[4] * m[9] + m[8] * m[5]) * reciprocalDeterminant;
    out[9] = (-m[0] * m[9] + m[8] * m[1]) * reciprocalDeterminant;
    out[10] = -(-m[0] * m[5] + m[4] * m[1]) * reciprocalDeterminant;
    out[11] = 0.0f;
    out[12] = (-m[4] * m[9] * m[14]
             + m[4] * m[10] * m[13]
             + m[8] * m[5] * m[14]
             - m[8] * m[6] * m[13]
             - m[12] * m[5] * m[10]
             + m[12] * m[9] * m[6]) * reciprocalDeterminant;
    out[13] = (m[0] * m[9] * m[14]
             - m[0] * m[10] * m[13]
             - m[8] * m[1] * m[14]
             + m[8] * m[2] * m[13]
             + m[12] * m[1] * m[10]
             - m[12] * m[9] * m[2]) * reciprocalDeterminant;
    inverseTranslationZNumeratorPartial = m[0] * m[5] * m[14]
                                        - m[0] * m[6] * m[13]
                                        - m[4] * m[1] * m[14]
                                        + m[4] * m[2] * m[13]
                                        + m[12] * m[1] * m[6];
    out[14] = (m[12] * m[5] * m[2] - inverseTranslationZNumeratorPartial) * reciprocalDeterminant;
    out[15] = one;
}
