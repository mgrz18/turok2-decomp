
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
extern s32 D_800D81C0;
extern s32 D_800D81C4;
void func_00202A08(u32 arg0)
{
  s32 *temp_v0;
  s32 *var_a0;
  s32 *var_a2;
  s32 *var_v1;
  var_v1 = D_800D81C4 + ((((((arg0 << 5) ^ (arg0 >> 1)) ^ (arg0 >> 9)) ^ (arg0 >> 0x11)) & D_800D81C0) * 0x10);
  var_a2 = 0;
  if ((*var_v1) != arg0)
  {
    loop_1:
    var_a2 = var_v1;

    var_v1 = *((s32 **) (((s8 *) var_v1) + 0xC));
    if (var_v1 != 0)
    {
 do { if ((*var_v1) == arg0) { goto block_3; } } while (0);
      goto loop_1;
    }
  }
  else
  {
    block_3:
    if (var_v1 != 0)
    {
      var_a0 = *((s32 **) (((s8 *) var_v1) + 0xC));
      if (var_a0 != 0)
      {
        *((s32 *) (((s8 *) var_v1) + 4)) = (s32) (*((s32 *) (((s8 *) var_a0) + 4)));
        *((s32 *) (((s8 *) var_v1) + 0)) = *((s32 *) (((s8 *) var_a0) + 0));
        *((s32 **) (((s8 *) var_v1) + 0xC)) = (s32 *) (*((s32 **) (((s8 *) var_a0) + 0xC)));
        temp_v0 = var_v1;
        var_v1 = var_a0;
        var_a0 = temp_v0;
      }
      if (var_a2 != 0)
      {
        *((s32 **) (((s8 *) var_a2) + 0xC)) = var_a0;
      }
      *((s32 *) (((s8 *) var_v1) + 4)) = 0;
      *((s32 *) (((s8 *) var_v1) + 0)) = 0;
      *((s32 *) (((s8 *) var_v1) + 0xC)) = 0;
    }

  }
}
