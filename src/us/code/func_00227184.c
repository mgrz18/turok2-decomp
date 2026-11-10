
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
#ifdef TARGET_PORT
/* host: the element addresses are 64-bit (Sort_Quick passes uptr) */
typedef unsigned long uptr_t;
void Sort_Insertion(uptr_t arg0, uptr_t arg1, u32 arg2, s32 (*arg3)(uptr_t, uptr_t), void (*arg4)(uptr_t, uptr_t))
{
  uptr_t var_s0;
  uptr_t var_s1;
  uptr_t var_s2 = arg1;
  while (arg0 < var_s2)
  {
    var_s1 = arg0;
    for (var_s0 = arg0 + arg2; var_s0 <= var_s2; var_s0 += arg2)
    {
      if (arg3(var_s0, var_s1) > 0)
      {
        var_s1 = var_s0;
      }
    }
    arg4(var_s1, var_s2);
    var_s2 -= arg2;
  }
}
#else
void Sort_Insertion(u32 arg0, u32 arg1, s32 arg2, s32 (*arg3)(u32, u32), s32 (*arg4)(u32, u32))
{
  u32 var_a0;
  u32 var_s0;
  u32 var_s1;
  u32 var_s2;
  var_s2 = arg1;
  if (arg0 < var_s2)
  {
    var_s0 = arg0 + arg2;
    do
    {
      if (arg0)
      {
        var_s1 = arg0;
      }
      else
      {
        var_s1 = arg0;
      }
      if (var_s2 >= var_s0)
      {
        var_a0 = var_s0;
        do
        {
          if (arg3(var_a0, var_s1) > 0)
          {
            var_s1 = var_s0;
          }
          var_s0 += arg2;
          var_a0 = var_s0;
        }
        while (var_s2 >= var_s0);
      }
      arg4(var_s1, var_s2);
      var_s2 -= arg2;
      var_s0 = arg0 + arg2;
    }
    while (arg0 < var_s2);
  }
}
#endif
