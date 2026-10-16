/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80274244_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
extern float D_800A5654;
void func_00211114(f32 *q, f32 *m)
{
  f32 x = q[0];
  f32 xx = x * x;
  f32 y = q[1];
  f32 yy = y * y;
  f32 z = q[2];
  f32 zz = z * z;
  f32 w = q[3];
  f32 ww = w * w;
  f32 x2 = x + x;
  f32 xy = x2 * y;
  f32 w2 = w + w;
  f32 wz = w2 * z;
  f32 xz = x2 * z;
  f32 wy = w2 * y;
  f32 yz = (y + y) * z;
  f32 r0 = ((ww + xx) - yy) - zz;
  f32 r1 = xy + wz;
  f32 r2 = xz - wy;
  f32 r4 = xy - wz;
  f32 r5 = ((ww - xx) + yy) - zz;
  f32 r6 = yz + (w2 * x);
  f32 r8 = xz + wy;
  f32 r9 = yz - (w2 * x);
  f32 r10 = ((ww - xx) - yy) + zz;
  m[3] = 0.0f;
  m[7] = 0.0f;
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = D_800A5654;
  m[0] = r0;
  m[1] = r1;
  m[2] = r2;
  m[4] = r4;
  m[5] = r5;
  m[6] = r6;
  m[8] = r8;
  m[9] = r9;
  m[10] = r10;
}
