/* From decompals/ultralib (libultra 2.0I): src/gu/ortho.c, guOrthoF.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */

extern void guMtxIdentF(float mf[4][4]);
extern void guOrthoF(float mf[4][4], float l, float r, float b, float t,
       float n, float f, float scale);
void guOrthoF(float mf[4][4], float l, float r, float b, float t, float n, float f, float scale)
{
 int i, j;
 guMtxIdentF(mf);
 mf[0][0] = 2/(r-l);
 mf[1][1] = 2/(t-b);
 mf[2][2] = -2/(f-n);
 mf[3][0] = -(r+l)/(r-l);
 mf[3][1] = -(t+b)/(t-b);
 mf[3][2] = -(f+n)/(f-n);
 mf[3][3] = 1;
 for (i=0; i<4; i++)
     for (j=0; j<4; j++)
  mf[i][j] *= scale;
}
