
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
s32 osSetIntMask(s32);
s32 func_002933B0(void *);
s32 func_002933E0(void *, void *);
void func_00293BA8(void *arg0, s32 arg1)
{
  long long new_var;
  s32 temp_s3;
  void *temp_s1;
  void *var_s0;
  temp_s3 = osSetIntMask(1);
  var_s0 = *((void **) (((s8 *) arg0) + 8));
  if (var_s0 != 0)
  {
    do
    {
      temp_s1 = *((void **) (((s8 *) var_s0) + 0));
      if ((*((s32 *) (((s8 *) var_s0) + 0x10))) == arg1)
      {
        new_var = 0;
        if (temp_s1 != new_var)
        {
          *((s32 *) (((s8 *) temp_s1) + 8)) = (s32) ((*((s32 *) (((s8 *) temp_s1) + 8))) + (*((s32 *) (((s8 *) var_s0) + 8))));
        }
        func_002933B0(var_s0);
        func_002933E0(var_s0, arg0);
      }
      var_s0 = temp_s1;
    }
    while (var_s0 != 0);
  }
  osSetIntMask(temp_s3);
}
