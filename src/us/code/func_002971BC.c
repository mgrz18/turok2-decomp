
#include "protos/ultra.h"
#ifdef TARGET_PORT
#include "structs/libaudio.h"
/* host: the low-pass filter's fields at the host offsets (fstate is a pointer at 0x28, first at 0x30) */
void *_filterBuffer(void *arg0, s32 arg1, s32 arg2, void *arg3)
{
  ALLowPass *lp = arg0;
  u32 *cmd = arg3;
  s32 buf = arg1 & 0xFFFF;
  cmd[0] = buf | 0x08000000;
  cmd[1] = (buf << 0x10) | ((arg2 * 2) & 0xFFFF);
  cmd[2] = 0x0B000020;
  cmd[3] = osVirtualToPhysical(&lp->fcvec);
  cmd[4] = ((lp->first & 0xFF) << 0x10) | (u16) lp->fgain | 0x0E000000;
  cmd[5] = osVirtualToPhysical(lp->fstate);
  lp->first = 0;
  return cmd + 6;
}
#else
void *_filterBuffer(void *arg0, s32 arg1, s32 arg2, void *arg3)
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
  *((s32 *) (((s8 *) var_s0) + 0xC)) = osVirtualToPhysical(var_s2 + 8);
  *((s32 *) (((s8 *) var_s0) + 0x10)) = (s32) (((*((u8 *) (((s8 *) var_s2) + 0x2F))) << 0x10) | ((*((u16 *) (((s8 *) var_s2) + 2))) | new_var2));
  var_s1 = var_s0 + 0x18;
  *((s32 *) (((s8 *) var_s0) + 0x14)) = osVirtualToPhysical(*((s32 *) (((s8 *) var_s2) + 0x28)));
  *((s32 *) ((new_var = (s8 *) var_s2) + 0x2C)) = 0;
  return var_s1;
}
#endif
