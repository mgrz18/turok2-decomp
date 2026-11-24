/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */
/* Boundary fixed in versions/cuts.us.txt: splat starts func_002975C4 three words early (stray words after
 * cosf); the function starts at 0x2975D0. */
/* From decompals/ultralib (libultra): src/gu/lookat.c, guLookAtF_2975D0 and guLookAt
 * (Turok 2's build uses float literals; guLookAtF_2975D0 is inlined). */

/* ultralib's float matrix (not rw/abu_types.h's struct Matrix) */
typedef float GuMatrix[4][4];
typedef struct { long m[4][4]; } Mtx;
void guMtxIdentF(float mf[4][4]);          /* guMtxIdentF */
void guMtxF2L(float mf[4][4], Mtx *m);  /* guMtxF2L */
float func_00298470(float x);                /* sqrtf */

static __inline__ void guLookAtF_2975D0(GuMatrix mf, float xEye, float yEye, float zEye,
                   float xAt, float yAt, float zAt,
                   float xUp, float yUp, float zUp) {
    float len, xLook, yLook, zLook, xRight, yRight, zRight;

    guMtxIdentF(mf);

    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;

    /* Negate because positive Z is behind us: */
    len = -1.0f / func_00298470(xLook * xLook + yLook * yLook + zLook * zLook);
    xLook *= len;
    yLook *= len;
    zLook *= len;

    /* Right = Up x Look */
    xRight = yUp * zLook - zUp * yLook;
    yRight = zUp * xLook - xUp * zLook;
    zRight = xUp * yLook - yUp * xLook;
    len = 1.0f / func_00298470(xRight * xRight + yRight * yRight + zRight * zRight);
    xRight *= len;
    yRight *= len;
    zRight *= len;

    /* Up = Look x Right */
    xUp = yLook * zRight - zLook * yRight;
    yUp = zLook * xRight - xLook * zRight;
    zUp = xLook * yRight - yLook * xRight;
    len = 1.0f / func_00298470(xUp * xUp + yUp * yUp + zUp * zUp);
    xUp *= len;
    yUp *= len;
    zUp *= len;

    mf[0][0] = xRight;
    mf[1][0] = yRight;
    mf[2][0] = zRight;
    mf[3][0] = -(xEye * xRight + yEye * yRight + zEye * zRight);

    mf[0][1] = xUp;
    mf[1][1] = yUp;
    mf[2][1] = zUp;
    mf[3][1] = -(xEye * xUp + yEye * yUp + zEye * zUp);

    mf[0][2] = xLook;
    mf[1][2] = yLook;
    mf[2][2] = zLook;
    mf[3][2] = -(xEye * xLook + yEye * yLook + zEye * zLook);

    mf[0][3] = 0;
    mf[1][3] = 0;
    mf[2][3] = 0;
    mf[3][3] = 1;
}

void guLookAt(Mtx *m, float xEye, float yEye, float zEye,
                   float xAt, float yAt, float zAt,
                   float xUp, float yUp, float zUp) {
    GuMatrix mf;

    guLookAtF_2975D0(mf, xEye, yEye, zEye, xAt, yAt, zAt, xUp, yUp, zUp);

    guMtxF2L(mf, m);
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/lib_802BB630/802BB740.CPP, func_802B6900_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
f32 func_00298470(f32);
extern f32 func_00298470(f32);

void guLookAtF(f32 matrix[4][4], f32 eyeX, f32 eyeY, f32 eyeZ,
                      f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ) {
    f32 lookX, lookY, lookZ;
    f32 rightX, rightY, rightZ;
    f32 scale, one;
    guMtxIdentF(matrix);
    lookX = atX - eyeX;
    lookY = atY - eyeY;
    lookZ = atZ - eyeZ;
    scale = -1.0f / func_00298470(lookX * lookX + lookY * lookY + lookZ * lookZ);
    lookX *= scale;
    lookZ *= scale;
    lookY *= scale;
    rightX = upY * lookZ - upZ * lookY;
    rightY = upZ * lookX - upX * lookZ;
    rightZ = upX * lookY - upY * lookX;
    one = 1.0f;
    scale = one / func_00298470(rightX * rightX + rightY * rightY + rightZ * rightZ);
    rightX *= scale;
    rightZ *= scale;
    rightY *= scale;
    upX = lookY * rightZ - lookZ * rightY;
    upY = lookZ * rightX - lookX * rightZ;
    upZ = lookX * rightY - lookY * rightX;
    scale = one / func_00298470(upX * upX + upY * upY + upZ * upZ);
    upX *= scale;
    upY *= scale;
    upZ *= scale;
    matrix[0][0] = rightX;
    matrix[1][0] = rightY;
    matrix[2][0] = rightZ;
    matrix[0][1] = upX;
    matrix[1][1] = upY;
    matrix[2][1] = upZ;
    matrix[0][2] = lookX;
    matrix[1][2] = lookY;
    matrix[2][2] = lookZ;
    matrix[3][0] = -(eyeX * rightX + eyeY * rightY + eyeZ * rightZ);
    matrix[3][1] = -(eyeX * upX + eyeY * upY + eyeZ * upZ);
    matrix[3][2] = -(eyeX * lookX + eyeY * lookY + eyeZ * lookZ);
    matrix[0][3] = 0.0f;
    matrix[1][3] = 0.0f;
    matrix[2][3] = 0.0f;
    matrix[3][3] = one;
}
