#include "common.h"
#include "m2c_macros.h"

s32 func_00220260(s32, s32, s32);

extern M2C_UNK D_800F7078;

void func_00266924(void *arg0)
{
  int new_var;
  new_var = ~0x2000;
  *((s32 *) (((s8 *) arg0) + 0xD4)) = (s32) (((*((s32 *) (((s8 *) arg0) + 0xD4))) & new_var) & (~0x100));
  func_00220260((s32) (&D_800F7078), (s32) arg0, 1);
}
