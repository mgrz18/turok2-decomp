
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
extern f32 D_800C052C;
extern f32 D_800C0530;
extern M2C_UNK D_8012F9E8;
void func_0044065C(void *arg0)
{
  unsigned int new_var3;
  f32 new_var4;
  s8 *new_var;
  f32 new_var2;
  new_var3 = (*((s32 *) (((s8 *) arg0) + 0xD4))) | 0x2000;
  new_var4 = D_800C052C;
  new_var = ((s8 *) arg0) + 0xD4;
  new_var2 = (f32) D_800C0530;
  *((s32 *) new_var) = (s32) new_var3;
  *((f32 *) (((s8 *) (&D_8012F9E8)) + 0)) = (f32) new_var4;
  *((f32 *) (((s8 *) (&D_8012F9E8)) + (-4))) = new_var2;
}
