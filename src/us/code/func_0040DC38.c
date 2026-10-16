
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
s32 func_00246108(s32, s32, s32, s32, s32);
void func_0040DC38(s32 arg0, s32 arg1)
{
  u8 sp18[0x88];
  func_00246108(arg0, arg1, (s32) (&sp18), 0, 1);
  if ((*((f32 *) (((s8 *) arg1) + 0x40))) > (*((f32 *) (((s8 *) arg1) + 0x64))))
  {
    func_00243414(arg0, arg1, 5);
  }
}
