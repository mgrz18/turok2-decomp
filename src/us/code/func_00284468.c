
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
s32 func_00283A4C(void);
M2C_UNK func_00275904(M2C_UNK);
extern void *D_800C1BB0;
void func_00284468(s32 arg0)
{
  M2C_UNK (*temp_v1)();
  if ((*((s32 *) (((s8 *) D_800C1BB0) + 0x44))) == 0)
  {
    func_00275904(-1);
    if (((*((s32 *) (((s8 *) D_800C1BB0) + 0x5C))) != 0) || (arg0 != 0))
    {
      func_00283A4C();
    }
    temp_v1 = *((M2C_UNK (**)()) (((s8 *) D_800C1BB0) + 0x10));
    if (temp_v1 != 0)
    {
      if (D_800C1BB0)
      {
        *((M2C_UNK (**)()) (((s8 *) D_800C1BB0) + 0x10)) = 0;
        temp_v1();
      }
      else
      {
        *((M2C_UNK (**)()) (((s8 *) D_800C1BB0) + 0x10)) = 0;
        temp_v1();
      }
    }
    *((s32 *) (((s8 *) D_800C1BB0) + 0x44)) = 1;
  }
}
