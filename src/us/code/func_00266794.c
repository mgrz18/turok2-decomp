#include "common.h"
#include "m2c_macros.h"

s32 func_00220408(s32, s32);
s32 func_00243414(s32, s32, s32);

extern M2C_UNK D_800F7078;

void func_00266794(void *arg0, void *arg1)
{
  int new_var;
  *((s32 *) (((s8 *) arg1) + 0x10C)) = (s32) (*((s32 *) (((s8 *) (*((void **) (((s8 *) arg0) + 0x14)))) + 0x18)));
  func_00243414((s32) arg0, (s32) arg1, 0);
  *((f32 *) (((s8 *) arg1) + 0x110)) = 1.0f;
  *((f32 *) (((s8 *) arg1) + 0x114)) = 1.0f;
  if (func_00220408((s32) (&D_800F7078), (s32) arg0) == 1)
  {
    new_var = ~0x2000;
    *((s32 *) (((s8 *) arg0) + 0xD4)) = (s32) (((*((s32 *) (((s8 *) arg0) + 0xD4))) & new_var) & (~0x100));
  }
}
