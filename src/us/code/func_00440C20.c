
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
M2C_UNK func_0027AD24(M2C_UNK *, s32, M2C_UNK *);
extern M2C_UNK D_004312D4;
extern M2C_UNK D_004315BC;
extern M2C_UNK D_800C046C;
extern M2C_UNK D_801100F0;
extern M2C_UNK D_8012FA24;
void func_00440C20(s32 arg0, s32 arg1)
{
  func_0027AD24(&D_801100F0, arg0, &D_800C046C);
  *((s32 *) (((s8 *) (&D_8012FA24)) + 0)) = -1;
  *((M2C_UNK **) (((s8 *) (&D_8012FA24)) + (-4))) = &D_004315BC;
  *((s8 *) (((s8 *) (&D_8012FA24)) + (-0xC))) = 1;
  *((s32 *) (((s8 *) (&D_8012FA24)) + 4)) = -1;
  *((s32 *) (((s8 *) (&D_8012FA24)) + (-0x14))) = -1;
  *((s32 *) (((s8 *) (&D_8012FA24)) + (-0x10))) = -1;
  *((M2C_UNK **) (((s8 *) (&D_8012FA24)) + (-0x18))) = &D_004312D4;
  func_00243414(arg0, arg1, 5);
}
