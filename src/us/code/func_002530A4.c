#include "common.h"
#include "m2c_macros.h"

s32 func_00267090(s32);

void func_002530A4(void *arg0, void *arg1)
{
  s32 temp_v1;
  s32 temp_v1_2;
  void *var_a0;
  void *var_s0;
  var_s0 = *((void **) (((s8 *) arg0) + 0x20));
  if (var_s0 != 0)
  {
    ;
    do
    {
      *((s32 *) (((s8 *) var_s0) + 0x54)) = 0;
      *((s32 *) (((s8 *) var_s0) + 0x2DC)) = 0;
      if (func_00267090((s32) var_s0) != 0)
      {
        temp_v1 = *((s32 *) (((s8 *) arg1) + 0x8EC));
        if (temp_v1 != 0x200)
        {
          *((void **) (((s8 *) (arg1 + (temp_v1 << 2))) + 0xEC)) = var_s0;
          *((s32 *) (((s8 *) arg1) + 0x8EC)) = (s32) (temp_v1 + 1);
        }
        temp_v1_2 = *((s32 *) (((s8 *) arg1) + 0xAF0));
        if (temp_v1_2 != 0x80)
        {
          *((void **) (((s8 *) (arg1 + (temp_v1_2 << 2))) + 0x8F0)) = var_s0;
          *((s32 *) (((s8 *) arg1) + 0xAF0)) = (s32) (temp_v1_2 + 1);
        }
      }
      var_s0 = *((void **) (((s8 *) var_s0) + 0x1320));
      var_a0 = var_s0;
    }
    while (var_s0 != 0);
  }
}
