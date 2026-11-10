
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
/* the elements are host pointers (docs/port/64BIT.md): u32 on the N64 */
typedef __UINTPTR_TYPE__ t2_uptr;
#else
typedef u32 t2_uptr;
#endif
void Sort_Insertion(t2_uptr arg0, t2_uptr arg1, s32 arg2, s32 (*arg3)(t2_uptr, t2_uptr), s32 (*arg4)(t2_uptr, t2_uptr))
{
  t2_uptr var_a0;
  t2_uptr var_s0;
  t2_uptr var_s1;
  t2_uptr var_s2;
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
