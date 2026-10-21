/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/lib_802BB630/802BB740.CPP, func_802B6900_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef float f32;
f32 func_00298470(f32);
extern float D_800AA7B8;
extern float D_800AA7BC;
extern void func_00297BCC(u8 *arg0);
extern f32 func_00298470(f32);
extern f32 D_800AA7B8, D_800AA7BC;
void func_00297860(f32 matrix[4][4], f32 eyeX, f32 eyeY, f32 eyeZ,
                      f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ) {
    f32 lookX, lookY, lookZ;
    f32 rightX, rightY, rightZ;
    f32 scale, one;
    func_00297BCC(matrix);
    lookX = atX - eyeX;
    lookY = atY - eyeY;
    lookZ = atZ - eyeZ;
    scale = D_800AA7B8 / func_00298470(lookX * lookX + lookY * lookY + lookZ * lookZ);
    lookX *= scale;
    lookZ *= scale;
    lookY *= scale;
    rightX = upY * lookZ - upZ * lookY;
    rightY = upZ * lookX - upX * lookZ;
    rightZ = upX * lookY - upY * lookX;
    one = D_800AA7BC;
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
