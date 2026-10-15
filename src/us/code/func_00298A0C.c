
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
extern s8 D_801213E0;
void func_00298A0C(void)
{
  s32 var_v0;
  s8 *var_v1;
  int new_var;
  var_v1 = &D_801213E0;
  *((s32 *) (((s8 *) (&D_801213E0)) + 0x3C)) = 1;
  new_var = 0xFD;
  var_v0 = 3;
  do
  {
    *var_v1 = new_var;
    var_v0 -= 1;
    var_v1 += 1;
  }
  while (var_v0 >= 0);
  var_v0 = 0xFE;
  *var_v1 = var_v0;
}
