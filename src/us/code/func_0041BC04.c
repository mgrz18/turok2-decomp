#include "common.h"
#include "m2c_macros.h"

s32 func_0041BC50(s32);

void func_0041BC04(void *arg0, s8 arg1)
{
  int new_var;
  new_var = 0x80;
  *((s8 *) (((s8 *) arg0) + 0x29)) = new_var;
  *((s8 *) (((s8 *) arg0) + 0x2A)) = new_var;
  *((s8 *) (((s8 *) arg0) + 0x28)) = 0;
  *((s8 *) (((s8 *) arg0) + 0x2C)) = 0;
  *((s8 *) (((s8 *) arg0) + 0x2B)) = 0;
  *((s8 *) (((s8 *) arg0) + 0x2D)) = 1;
  *((s8 *) (((s8 *) arg0) + 0x2E)) = 0;
  *((s8 *) (((s8 *) arg0) + 0x2F)) = arg1;
  *((s8 *) (((s8 *) arg0) + 0x30)) = 0;
  *((s8 *) (((s8 *) arg0) + 0x31)) = 1;
  *((s8 *) (((s8 *) arg0) + 0x32)) = 0;
  func_0041BC50((s32) arg0);
}
