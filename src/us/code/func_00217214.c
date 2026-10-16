/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025E590_de_8.c, func_8025FD74_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;
struct Curve;
typedef struct Curve Curve;
struct Curve;
struct Curve {
    s32 unk0;
    f32 unk4;
    f32 unk8;
    s32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u32 unk20;
    s32 unk24;
    s32 unk28;
};
s32 func_00217C58(s32 *, s32);
void func_00217214(Curve *arg0)
{
  s32 sp10;
  int new_var;
  f32 temp_f0;
  f32 temp_f0_2;
  f32 temp_f0_3;
  f32 temp_f4;
  f32 temp_f5;
  f32 temp_f6;
  f32 temp_f7;
  f32 var_f1;
  f32 var_f3;
  f64 var_f2;
  s32 temp_a2;
  s32 temp_lo;
  s32 temp_s3;
  s32 temp_s0;
  s32 temp_s1;
  s32 temp_s2;
  u32 temp_s5;
  s32 temp_v0;
  temp_s3 = arg0->unk0;
  temp_lo = arg0->unk28 * ((temp_s3 * 4) + 6);
  sp10 = arg0->unkC + temp_lo;
  temp_s5 = func_00217C58(&sp10, 6);
  temp_s1 = func_00217C58(&sp10, temp_s3);
  temp_s0 = func_00217C58(&sp10, temp_s3);
  temp_s2 = func_00217C58(&sp10, temp_s3);
  temp_v0 = func_00217C58(&sp10, temp_s3);
  temp_f6 = (f32) temp_s1;
  new_var=temp_s1*0x15;
  temp_a2 = temp_s0 << 5;
  temp_f5 = (f32) (((temp_a2 - new_var) - (temp_s2 * 0xC)) + temp_v0);
  temp_f5 *= 0.33333334f;
  temp_f7 = (f32) ((((temp_s1 * 0xE) - temp_a2) + (temp_s2 * 0x14)) - (temp_v0 * 2));
  new_var = temp_s1 * 0x18;
  temp_f4 = (f32) ((((temp_s0 << 6) - new_var) - (temp_s2 * 0x30)) + (temp_v0 * 8));
  temp_f4 *= 0.33333334f;
  if (temp_s3 != 0)
  {
    var_f3 = arg0->unk8 / ((f32) ((1 << temp_s3) - 1));
  }
  else
  {
    temp_f0 = arg0->unk8 * 0.5f;
    arg0->unk8 = 0.0f;
    var_f3 = arg0->unk8;
    arg0->unk4 = (f32) (arg0->unk4 + temp_f0);
  }
  temp_f6 *= var_f3;
  if (temp_s5 >= 2U)
  {
    var_f2 = (f64) temp_s5;
    var_f1 = 1.0f / (((f32) var_f2) - 1.0f);
  }
  else
  {
    var_f1 = 1.0f;
  }
  temp_f0_2 = var_f1 * var_f3;
  temp_f5 *= temp_f0_2;
  temp_f0_2 *= var_f1;
  temp_f7 *= temp_f0_2;
  temp_f0_2 *= var_f1;
  temp_f6 += arg0->unk4;
  temp_f4 *= temp_f0_2;
  arg0->unk20 = temp_s5;
  arg0->unk14 = temp_f5;
  arg0->unk10 = temp_f6;
  arg0->unk18 = temp_f7;
  arg0->unk1C = temp_f4;
}
