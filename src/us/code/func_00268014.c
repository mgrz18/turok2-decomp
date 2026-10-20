
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
s32 func_00268014(void *arg0)
{
  void *temp_a0;
  s8 *new_var2;
  s32 *new_var;
  s8 *new_var3;
  int new_var4;
  temp_a0 = *((void **) (((s8 *) arg0) + 0x14));
  new_var3 = (s8 *) temp_a0;
  new_var4 = 1;
  new_var = (s32 *) (new_var3 + 0);
  if ((*new_var) != new_var4)
  {
    if (new_var4)
    {
      return 0;
    }
  }
 do { } while (0);
  ;
  return (*((s32 *) (new_var3 - -0x48))) & 0x1000;
}
