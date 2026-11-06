#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"
#include "structs/region.h"

void CPlayer__UpdateOxygen(void *, void *);                 /* extern */
void CPlayer__UpdateSubmerged(void *, void *);                 /* extern */

void CPlayer__UpdateWaterRegion(CPlayer *arg0, CGameObjectInstance *arg1, CRegionInfo *arg2)
{
  int new_var;
  s32 temp_s0;
  s32 temp_s2;
  s32 temp_v1;
  if (arg2 != 0)
  {
    temp_v1 = arg1->m_AI.m_dwFlags;
    temp_s2 = arg2->m_wFlags52 & 2;
    new_var = temp_v1 & 0x100;
    temp_s0 = temp_v1 & 0x80;
    if (temp_s0 == 0)
    {
      arg0->m_Oxygen = 0;
    }
    if ((!new_var) && (temp_s2 != 0))
    {
      CPlayer__UpdateSubmerged(arg0, arg1);
    }
    if ((temp_s0 != 0) && (temp_s2 == 0))
    {
      CPlayer__UpdateOxygen(arg0, arg1);
    }
  }
}
