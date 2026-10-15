
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
s32 func_0026EFB8(s32);
s32 func_00416644(s32, s32, s32, s32, s32);
extern M2C_UNK D_0043CAB4;
extern s32 D_80110048;
extern M2C_UNK D_80119E2C;
void func_002578FC(s32 arg0, void *arg1)
{
  int new_var;
  u32 temp_a0;
  if ((*((s32 *) (((s8 *) arg1) + 0xC00))) != 0)
  {
    temp_a0 = func_0026EFB8(D_80110048) - (new_var = 1);
    if ((temp_a0 < 6U) && ((*((s32 *) (((s8 *) arg1) + 0x9F8))) & (new_var << temp_a0)))
    {
      func_00416644((s32) (&D_80119E2C), (s32) (&D_0043CAB4), (s32) arg1, *((s32 *) (((s8 *) arg1) + 0xA50)), 0);
    }
    *((s32 *) (((s8 *) arg1) + 0xC00)) = 0;
  }
}
