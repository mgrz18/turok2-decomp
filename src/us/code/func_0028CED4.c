
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
extern u16 sCrashBgColor;
extern s32 gScreenWidth;
extern s32 gScreenHeight;
extern u16 *sCrashFrameBuffer;
void Crash_ClearScreen(void)
{
  s32 var_a0;
  u16 *var_a1;
  int new_var;
  var_a1 = sCrashFrameBuffer;
  var_a0 = 0;
  new_var = 0;
  if ((gScreenWidth * gScreenHeight) > new_var)
  {
    do
    {
      *var_a1 = sCrashBgColor;
      var_a0 += 1;
      var_a1 = (typeof(var_a1)) ((s8 *) ((typeof(var_a1)) (((s8 *) var_a1) + 2)));
    }
    while (var_a0 < (gScreenWidth * gScreenHeight));
  }
}
