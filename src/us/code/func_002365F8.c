
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
M2C_UNK CList__AddTail(void *, void *);
M2C_UNK CList__Remove(void **, void *);
void func_002365F8(void *arg0, s32 arg1)
{
  s32 var_s3;
  int new_var;
  s32 var_s4;
  void **temp_s2;
  void *temp_s1;
  void *var_s0;
  if ((*((s32 *) (((s8 *) arg0) + 0x95B0))) != 0x64)
  {
    var_s4 = 0;
    new_var = 0x95A0;
    var_s3 = 0x94D8;
    do
    {
      temp_s2 = arg0 + var_s3;
      var_s0 = *temp_s2;
      if (var_s0 != 0)
      {
        do
        {
          temp_s1 = *((void **) (((s8 *) var_s0) + 4));
          if ((*((s32 *) (((s8 *) var_s0) + 0x48))) == arg1)
          {
            CList__Remove(temp_s2, var_s0);
            CList__AddTail(arg0 + new_var, var_s0);
          }
          var_s0 = temp_s1;
        }
        while (var_s0 != 0);
      }
      var_s4 += 1;
      var_s3 += 0x14;
    }
    while (var_s4 < 0xA);
  }
}
