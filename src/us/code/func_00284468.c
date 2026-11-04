
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
s32 func_00283A4C(void);
s32 func_00275904(s32);
extern void *D_800C1BB0;
void func_00284468(s32 arg0)
{
  s32 (*temp_v1)();
  if ((*((s32 *) (((s8 *) D_800C1BB0) + 0x44))) == 0)
  {
    func_00275904(-1);
    if (((*((s32 *) (((s8 *) D_800C1BB0) + 0x5C))) != 0) || (arg0 != 0))
    {
      func_00283A4C();
    }
    temp_v1 = *((s32 (**)()) (((s8 *) D_800C1BB0) + 0x10));
    if (temp_v1 != 0)
    {
      if (D_800C1BB0)
      {
        *((s32 (**)()) (((s8 *) D_800C1BB0) + 0x10)) = 0;
        temp_v1();
      }
      else
      {
        *((s32 (**)()) (((s8 *) D_800C1BB0) + 0x10)) = 0;
        temp_v1();
      }
    }
    *((s32 *) (((s8 *) D_800C1BB0) + 0x44)) = 1;
  }
}
