/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */
/* Boundary fixed in versions/cuts.us.txt: splat starts func_00297FAC one word early (a stray word); the
 * function starts at 0x297FB0. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/guPerspective.c, guPerspective, with Turok 2's layouts.
 * (decompals/ultralib src/gu/perspective.c with float literals; guPerspectiveF is inlined.) */

typedef unsigned short u16;
typedef float Matrix[4][4];
typedef struct { long m[4][4]; } Mtx;

void func_00297BCC(float mf[4][4]);          /* guMtxIdentF */
void func_00297C1C(float mf[4][4], Mtx *m);  /* guMtxF2L */
float func_002974C0(float x);                /* cosf */
float func_002982F0(float x);                /* sinf */

static __inline__ void guPerspectiveF(float mf[4][4], u16 *perspNorm, float fovy, float aspect,
                                      float near, float far, float scale) {
    float cot;
    int i, j;

    func_00297BCC(mf);

    fovy *= 3.1415926f / 180.0f;
    cot = func_002974C0(fovy / 2.0f) / func_002982F0(fovy / 2.0f);

    mf[0][0] = cot / aspect;
    mf[1][1] = cot;
    mf[2][2] = (near + far) / (near - far);
    mf[2][3] = -1;
    mf[3][2] = (2 * near * far) / (near - far);
    mf[3][3] = 0;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mf[i][j] *= scale;
        }
    }

    if (perspNorm != 0) {
        if (near + far <= 2.0f) {
            *perspNorm = 0xFFFF;
        } else {
            *perspNorm = (u16)((2.0f * 65536.0f) / (near + far));
            if (*perspNorm <= 0) {
                *perspNorm = 1;
            }
        }
    }
}

void func_00297FB0(Mtx *m, u16 *perspNorm, float fovy, float aspect, float near, float far, float scale) {
    Matrix mf;

    guPerspectiveF(mf, perspNorm, fovy, aspect, near, far, scale);
    func_00297C1C(mf, m);
}
