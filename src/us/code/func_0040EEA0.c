
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
s32 func_0020A33C(s32, s32, s32, s32, s32, s32, s32);
s32 func_0021D7BC(s32, s32, s32);
s32 func_00235898(void);
M2C_UNK func_00236324(void *);
extern s32 D_800AB8B0;
extern s32 D_800B6D1C;
extern M2C_UNK D_80130200;
void func_0040EEA0(s32 arg0, s32 arg1, s32 arg2)
{
  int new_var;
  s8 *new_var3;
  s8 *new_var2;
  new_var3 = &D_80130200;
  func_0021D7BC(arg0, arg1, arg2);
  if ((*((s32 *) (((s8 *) arg1) + 4))) != 0)
  {
    func_00236324((((*((s32 *) (((s8 *) arg1) + 0x114))) * 0x64) - -0x3C) + new_var3);
    func_00235898();
    new_var = 0x110;
    new_var3 = ((s8 *) arg0) + 0x98;
    func_0020A33C(*((s32 *) (((s8 *) arg2) + 0xC)), *((s32 *) new_var3), 1, arg0 + ((D_800B6D1C * 0x18) + new_var), 0, 1, (s32) (*((s8 *) (new_var2 = ((s8 *) arg0) + 0x151))));
  }
  D_800AB8B0 = 0;
}
