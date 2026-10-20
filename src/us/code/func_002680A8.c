
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
s32 func_002680A8(void *arg0)
{
  s8 *new_var2;
  unsigned int new_var3;
  void *temp_a0;
  void **new_var;
  new_var = (void **) (((s8 *) arg0) + 0x14);
  temp_a0 = *new_var;
  if ((*((s32 *) ((new_var2 = (s8 *) temp_a0) + 0))) != 1)
  {
    return 0;
 new_var3 = 0; do { } while (new_var3);
  }
  return (*((s32 *) (new_var2 + 0x48))) & 0x200;
}
