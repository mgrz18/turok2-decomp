#include "common.h"
#include "globals/camera.h"
#include "protos/list.h"
#include "structs/camera.h"

/* The main camera (pool + 0x40) and each active camera give their active texts (CCamera
 * m_ActiveTextList, 0xE28) back to the pool's free texts (m_FreeTextList, 0xF0C). */
#define TEXTS T2_OFFSETOF(CCamera, m_ActiveTextList)
#define FREE_TEXTS T2_OFFSETOF(CCameraPool, m_FreeTextList)
#define MAIN_CAMERA T2_OFFSETOF(CCameraPool, m_MainCamera)



void CCameraPool__ReleaseAllTexts(sptr arg0)
{
  s32 *temp_s0;
  s32 *temp_s0_2;
  s32 *var_s1;
  s32 *var_s1_2;
  void *temp_s2;
  void *var_a0;
  void *var_a0_2;
  void *var_s2;
  temp_s2 = arg0 + MAIN_CAMERA;
  var_s1 = *((s32 **) (((s8 *) temp_s2) + TEXTS + T2_OFFSETOF(CList, m_pTail)));
  var_a0 = temp_s2 + TEXTS;
  if (var_s1 != 0)
  {
    do
    {
      temp_s0 = *var_s1;
      CList__Remove((sptr) (temp_s2 + TEXTS), var_s1);
      CList__AddHead(arg0 + FREE_TEXTS, var_s1);
      var_s1 = temp_s0;
      var_a0 = temp_s2 + TEXTS;
    }
    while (var_s1 != 0);
  }
  var_s2 = gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_pHead;
  if (var_s2 != 0)
  {
    do
    {
      var_s1_2 = *((s32 **) (((s8 *) var_s2) + TEXTS + T2_OFFSETOF(CList, m_pTail)));
      var_a0_2 = var_s2 + TEXTS;
      if (var_s1_2 != 0)
      {
        do
        {
          temp_s0_2 = *var_s1_2;
          CList__Remove(var_a0_2, var_s1_2);
          CList__AddHead(arg0 + FREE_TEXTS, var_s1_2);
          var_s1_2 = temp_s0_2;
          var_a0_2 = var_s2 + TEXTS;
        }
        while (var_s1_2 != 0);
      }
      var_s2 = *((void **) (((s8 *) var_s2) + 4));
    }
    while (var_s2 != 0);
  }
}
