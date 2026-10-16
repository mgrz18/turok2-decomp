/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BCC90_de_3.c, func_802B6F9C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
extern float func_002974C0(float x);
extern float func_002982F0(float x);
extern const float D_800AA818;
extern const float D_800AA81C;
extern const float D_800AA820;
extern const float D_800AA828;
extern void func_00297BCC(float mf[4][4]);
void func_0029815C(float mf[4][4], u16 *perspNorm, float fovy, float aspect, float near, float far, float scale)
{
    float cot;
    int i, j;
    float norm;
    unsigned int value;
    func_00297BCC(mf);
    fovy *= D_800AA818;
    fovy *= D_800AA81C;
    cot = func_002974C0(fovy) / func_002982F0(fovy);
    mf[0][0] = cot / aspect;
    mf[1][1] = cot;
    mf[2][2] = (near + far) / (near - far);
    mf[2][3] = D_800AA820;
    mf[3][2] = (2 * near * far) / (near - far);
    mf[3][3] = 0;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            mf[i][j] *= scale;
    if (perspNorm != (u16 *)0) {
        if (near + far <= (*(&D_800AA820 + 1))) {
            *perspNorm = (u16)0xFFFF;
        } else {
            norm = D_800AA828 / (near + far);
            if (norm >= (*(&D_800AA828 + 1))) {
                goto large;
            }
            value = (int)norm;
            goto converted;
        large:
            value = (int)(norm - (*(&D_800AA828 + 1)));
            value |= 0x80000000;
        converted:
            *perspNorm = (u16)value;
            if (*perspNorm <= 0)
                *perspNorm = (u16)0x0001;
        }
    }
}
