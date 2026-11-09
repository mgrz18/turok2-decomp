#include "common.h"
#include "globals/camera.h"
#include "protos/list.h"



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
      CList__Remove((sptr) (temp_s2 + 0xE28), var_s1);
      CList__AddHead(arg0 + 0xF0C, var_s1);
      var_s1 = temp_s0;
      var_a0 = temp_s2 + 0xE28;
    }
    while (var_s1 != 0);
  }
  var_s2 = gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_pHead;
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
          CList__Remove(var_a0_2, var_s1_2);
          CList__AddHead(arg0 + 0xF0C, var_s1_2);
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
