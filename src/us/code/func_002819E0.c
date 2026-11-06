#include "common.h"

s32 CList__AddHead(s32, s32);
s32 CList__Remove(s32, s32);

extern void *D_801198D0;

void CCameraPool__ReleaseAllTexts(s32 arg0)
{
  s32 *temp_s0;
  s32 *temp_s0_2;
  s32 *var_s1;
  s32 *var_s1_2;
  void *temp_s2;
  void *var_a0;
  void *var_a0_2;
  void *var_s2;
  temp_s2 = arg0 + 0x40;
  var_s1 = *((s32 **) (((s8 *) temp_s2) + 0xE2C));
  var_a0 = temp_s2 + 0xE28;
  if (var_s1 != 0)
  {
    do
    {
      temp_s0 = *var_s1;
      CList__Remove((s32) (temp_s2 + 0xE28), (s32) var_s1);
      CList__AddHead(arg0 + 0xF0C, (s32) var_s1);
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
      var_s1_2 = *((s32 **) (((s8 *) var_s2) + 0xE2C));
      var_a0_2 = var_s2 + 0xE28;
      if (var_s1_2 != 0)
      {
        do
        {
          temp_s0_2 = *var_s1_2;
          CList__Remove((s32) var_a0_2, (s32) var_s1_2);
          CList__AddHead(arg0 + 0xF0C, (s32) var_s1_2);
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
