#include "common.h"
#include "m2c_macros.h"

s32 func_002052D8(s32, s32);

extern void *D_800C1BB0;

void func_0028441C(void)
{
  s32 temp_a1;
  temp_a1 = *((s32 *) (((s8 *) D_800C1BB0) + 0));
  if (temp_a1 != 0)
  {
    func_002052D8(0, temp_a1);
  }
  *((s32 *) (((s8 *) D_800C1BB0) + 0x60)) = (*((s32 *) (((s8 *) D_800C1BB0) + 0x3C)) = (*((s32 *) (((s8 *) D_800C1BB0) + 0x38)) = (*((s32 *) (((s8 *) D_800C1BB0) + 4)) = (*((s32 *) (((s8 *) D_800C1BB0) + 0)) = 0))));
}
