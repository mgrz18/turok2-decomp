
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
s32 func_00243414(s32, s32, s32);
extern f32 D_800B6D28;
void func_00264574(void *arg0, void *arg1)
{
  s32 temp_a3;
  s32 var_a2;
  void *temp_t0;
  unsigned int new_var;
  temp_t0 = (*((s32 *) (((s8 *) arg0) + 0x14))) + 0x14;
  temp_a3 = *((s32 *) (((s8 *) temp_t0) + 0x24));
  var_a2 = 1;
  if (temp_a3 & 0x10000)
  {
    new_var = (*((s32 *) (((s8 *) arg1) + 0))) & 0x20000;
    var_a2 = new_var < 1;
  }
  if ((temp_a3 & 0x4000) && (!((*((s32 *) (((s8 *) arg1) + 0))) & 0x20000)))
  {
    var_a2 = 0;
  }
  if (var_a2 != 0)
  {
    *((f32 *) (((s8 *) arg1) + 0x64)) = (*((f32 *) (((s8 *) arg1) + 0x64))) + (D_800B6D28 / (*((f32 *) (((s8 *) temp_t0) + 0x38))));
  }
  if ((*((f32 *) (((s8 *) arg1) + 0x64))) >= 1.0f)
  {
    func_00243414(arg0, arg1, 2);
  }
}
