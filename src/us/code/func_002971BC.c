
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
s32 func_0029BF80(s32);
void *func_002971BC(void *arg0, s32 arg1, s32 arg2, void *arg3)
{
  s32 temp_a1;
  void *var_s0;
  int new_var2;
  void *var_s1;
  s8 *new_var;
  void *var_s2;
  new_var2 = 0x0E000000;
  var_s2 = arg0;
  var_s0 = arg3;
  temp_a1 = arg1 & 0xFFFF;
  *((s32 *) (((s8 *) var_s0) + 0)) = (s32) (temp_a1 | 0x08000000);
  *((s32 *) (((s8 *) var_s0) + 4)) = (s32) ((temp_a1 << 0x10) | ((arg2 * 2) & 0xFFFF));
  *((s32 *) (((s8 *) var_s0) + 8)) = 0x0B000020;
  *((s32 *) (((s8 *) var_s0) + 0xC)) = func_0029BF80(var_s2 + 8);
  *((s32 *) (((s8 *) var_s0) + 0x10)) = (s32) (((*((u8 *) (((s8 *) var_s2) + 0x2F))) << 0x10) | ((*((u16 *) (((s8 *) var_s2) + 2))) | new_var2));
  var_s1 = var_s0 + 0x18;
  *((s32 *) (((s8 *) var_s0) + 0x14)) = func_0029BF80(*((s32 *) (((s8 *) var_s2) + 0x28)));
  *((s32 *) ((new_var = (s8 *) var_s2) + 0x2C)) = 0;
  return var_s1;
}
