/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BCC90_de_3.c, func_802B6F9C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float func_002974C0(float x);
extern float func_002982F0(float x);
extern void guMtxIdentF(float mf[4][4]);
void guPerspectiveF(float mf[4][4], u16 *perspNorm, float fovy, float aspect, float near, float far, float scale)
{
    float cot;
    int i, j;
    float norm;
    unsigned int value;
    guMtxIdentF(mf);
    fovy *= 0.017453292f;
    fovy *= 0.5f;
    cot = func_002974C0(fovy) / func_002982F0(fovy);
    mf[0][0] = cot / aspect;
    mf[1][1] = cot;
    mf[2][2] = (near + far) / (near - far);
    mf[2][3] = -1.0f;
    mf[3][2] = (2 * near * far) / (near - far);
    mf[3][3] = 0;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            mf[i][j] *= scale;
    if (perspNorm != (u16 *)0) {
        if (near + far <= (2.0f)) {
            *perspNorm = (u16)0xFFFF;
        } else {
            norm = 131072.0f / (near + far);
            if (norm >= (2.1474836e9f)) {
                goto large;
            }
            value = (int)norm;
            goto converted;
        large:
            value = (int)(norm - (2.1474836e9f));
            value |= 0x80000000;
        converted:
            *perspNorm = (u16)value;
            if (*perspNorm <= 0)
                *perspNorm = (u16)0x0001;
        }
    }
}
