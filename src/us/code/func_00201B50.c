
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
s32 osJamMesg(s32, s32, s32);
s32 osRecvMesg(s32, s32, s32);
s32 func_0029B9B0(s32, s32);
M2C_UNK func_0029B030(M2C_UNK *, M2C_UNK *, M2C_UNK);
extern s32 D_800B6E54;
extern s32 D_800B6E88;
s32 func_00201B50(s32 arg0, void *arg1)
{
  s32 new_var3;
  u8 sp10[0x18];
  M2C_UNK sp28;
  unsigned char new_var2;
  M2C_UNK sp2C;
  M2C_UNK *new_var;
  new_var2 = 1;
  new_var = &sp28;
  func_0029B030(sp10, new_var, new_var2);
  new_var3 = D_800B6E88;
  *((M2C_UNK **) (((s8 *) arg1) + 0x20)) = sp10;
  D_800B6E54 = 2;
  func_0029B9B0(arg0, new_var3);
  osJamMesg(arg0 + 0x230, (s32) arg1, 1);
  osRecvMesg((s32) sp10, (s32) (&sp2C), new_var2);
  return 1;
}
