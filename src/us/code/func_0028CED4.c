
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
extern u16 D_800B7498;
extern s32 D_800C2030;
extern s32 D_800C2034;
extern u16 *D_8011F180;
void func_0028CED4(void)
{
  s32 var_a0;
  u16 *var_a1;
  int new_var;
  var_a1 = D_8011F180;
  var_a0 = 0;
  new_var = 0;
  if ((D_800C2030 * D_800C2034) > new_var)
  {
    do
    {
      *var_a1 = D_800B7498;
      var_a0 += 1;
      var_a1 = (typeof(var_a1)) ((s8 *) ((typeof(var_a1)) (((s8 *) var_a1) + 2)));
    }
    while (var_a0 < (D_800C2030 * D_800C2034));
  }
}
