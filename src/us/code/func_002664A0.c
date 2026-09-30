#include "common.h"
#include "m2c_macros.h"

void func_002664A0(void *arg0)
{
  int new_var;
  new_var = ~0x2000;
  if ((*((f32 *) (((s8 *) arg0) + 0x180))) > 30.0f)
  {
    *((s32 *) (((s8 *) arg0) + 0xD4)) = (s32) (((*((s32 *) (((s8 *) arg0) + 0xD4))) & new_var) & (~0x100));
  }
}
