
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
typedef s32 M2C_UNK;
typedef s8 M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;
s32 func_00279028(s32);
M2C_UNK func_0027B320(M2C_UNK, s32, s16, s16, f32, f32, s32);
extern f32 D_800A711C;
extern s32 D_800C2030;
extern s32 D_800C2034;
extern u8 D_8011AAEA;
void func_00254768(void *arg0, void *arg1)
{
  f32 temp_f0;
  f32 temp_f2;
  float new_var2;
  f32 temp_f3;
  f32 temp_f5;
  float new_var;
  func_00279028((s32) D_8011AAEA);
  temp_f0 = *((f32 *) (((s8 *) arg1) + 0x288));
  temp_f3 = temp_f0 / ((f32) D_800C2030);
  temp_f5 = *((f32 *) (((s8 *) arg1) + 0x28C));
  temp_f2 = temp_f5 / ((f32) D_800C2034);
  new_var = (*((f32 *) (((s8 *) arg1) + 0x290))) + temp_f0;
  new_var2 = (*((f32 *) (((s8 *) arg1) + 0x294))) + temp_f5;
  func_0027B320(0x1FC, (s32) (*((f32 *) (((s8 *) arg0) + 0xB60))), (s16) ((s32) (new_var + (temp_f3 * D_800A711C))), (s16) ((s32) (new_var2 + (temp_f2 * D_800A711C))), temp_f3, temp_f2, 1);
}
