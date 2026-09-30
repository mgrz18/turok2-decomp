#include "common.h"
#include "m2c_macros.h"

s32 func_00256B24(s32 arg0, s32 arg1)
{
  unsigned int new_var;
  new_var = (*((u8 *) (((s8 *) (arg0 + arg1)) + 0x52C))) & 0xF;
  if (arg1 || arg0)
  {
    return (*((u8 *) (((s8 *) (arg0 + arg1)) + 0x52C))) & 0xF;
  }
  else
  {
    return new_var;
  }
}
