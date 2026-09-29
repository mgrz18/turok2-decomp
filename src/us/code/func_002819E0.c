#include "common.h"
#include "m2c_macros.h"

s32 func_00200518(s32, s32);
s32 func_00200738(s32, s32);

extern void *D_801198D0;

void func_002819E0(s32 arg0)
{
  M2C_UNK *temp_s0;
  M2C_UNK *temp_s0_2;
  M2C_UNK *var_s1;
  M2C_UNK *var_s1_2;
  void *temp_s2;
  void *var_a0;
  void *var_a0_2;
  void *var_s2;
  temp_s2 = arg0 + 0x40;
  var_s1 = *((M2C_UNK **) (((s8 *) temp_s2) + 0xE2C));
  var_a0 = temp_s2 + 0xE28;
  if (var_s1 != 0)
  {
    do
    {
      temp_s0 = *var_s1;
      func_00200738((s32) (temp_s2 + 0xE28), (s32) var_s1);
      func_00200518(arg0 + 0xF0C, (s32) var_s1);
      var_s1 = temp_s0;
      var_a0 = temp_s2 + 0xE28;
    }
    while (var_s1 != 0);
  }
  var_s2 = D_801198D0;
  if (var_s2 != 0)
  {
    do
    {
      var_s1_2 = *((M2C_UNK **) (((s8 *) var_s2) + 0xE2C));
      var_a0_2 = var_s2 + 0xE28;
      if (var_s1_2 != 0)
      {
        do
        {
          temp_s0_2 = *var_s1_2;
          func_00200738((s32) var_a0_2, (s32) var_s1_2);
          func_00200518(arg0 + 0xF0C, (s32) var_s1_2);
          var_s1_2 = temp_s0_2;
          var_a0_2 = var_s2 + 0xE28;
        }
        while (var_s1_2 != 0);
      }
      var_s2 = *((void **) (((s8 *) var_s2) + 4));
    }
    while (var_s2 != 0);
  }
}
