/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */
/* From decompals/ultralib (libultra): src/gu/ortho.c, guOrtho with guOrthoF
 * inlined (guOrthoF itself is func_00297E8C). */

typedef float Matrix[4][4];
typedef struct { long m[4][4]; } Mtx;

void func_00297BCC(float mf[4][4]);          /* guMtxIdentF */
void func_00297C1C(float mf[4][4], Mtx *m);  /* guMtxF2L */

static __inline__ void guOrthoF(float mf[4][4], float l, float r, float b, float t,
                                float n, float f, float scale) {
    int i, j;

    func_00297BCC(mf);

    mf[0][0] = 2 / (r - l);
    mf[1][1] = 2 / (t - b);
    mf[2][2] = -2 / (f - n);
    mf[3][0] = -(r + l) / (r - l);
    mf[3][1] = -(t + b) / (t - b);
    mf[3][2] = -(f + n) / (f - n);
    mf[3][3] = 1;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            mf[i][j] *= scale;
}

void func_00297D50(Mtx *m, float l, float r, float b, float t,
                   float n, float f, float scale) {
    Matrix mf;

    guOrthoF(mf, l, r, b, t, n, f, scale);

    func_00297C1C(mf, m);
}
