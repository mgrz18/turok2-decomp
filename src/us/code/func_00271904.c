
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
s32 osRecvMesg(s32, s32, s32);
s32 osSendMesg(s32, s32, s32);
s32 func_0029DFF0(void);
void func_0029E010(s32);
s32 func_00271904(void *arg0, s32 arg1)
{
  s32 temp_a0;
  s32 temp_v0;
  s32 temp_v1;
  s32 temp_v1_2;
  s32 var_a0;
  s32 var_s1;
  void *temp_s0;
  void *temp_s0_2;
  void *var_v1;
  temp_s0 = arg0 + 0x110;
  var_s1 = 0;
  temp_a0 = func_0029DFF0();
  temp_v1 = (*((s32 *) (((s8 *) temp_s0) + 0x1C))) + 1;
  *((s32 *) (((s8 *) temp_s0) + 0x1C)) = temp_v1;
  if (temp_v1 != 1)
  {
    func_0029E010(temp_a0);
    osRecvMesg((s32) temp_s0, 0, 1);
    var_a0 = 0;
  }
  else
  {
    func_0029E010(temp_a0);
    var_a0 = 0;
  }
 do { var_v1 = arg0; } while (0);
  do
  {
    if ((*((s32 *) (((s8 *) var_v1) + 0x1DC0))) == arg1)
    {
      var_s1 += 1;
    }
    var_a0 += 1;
    var_v1 += 0xCC;
  }
  while (var_a0 < 0x11);
  temp_s0_2 = arg0 + 0x110;
  temp_v0 = func_0029DFF0();
  temp_v1_2 = (*((s32 *) (((s8 *) temp_s0_2) + 0x1C))) - 1;
  *((s32 *) (((s8 *) temp_s0_2) + 0x1C)) = temp_v1_2;
  if (temp_v1_2 != 0)
  {
    func_0029E010(temp_v0);
    osSendMesg((s32) temp_s0_2, 0, 1);
    return var_s1;
  }
  if (arg0)
  {
    func_0029E010(temp_v0);
    return var_s1;
  }
  else
  {
    func_0029E010(temp_v0);
    return var_s1;
  }
}
