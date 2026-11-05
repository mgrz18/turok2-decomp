#include "common.h"
#include "structs/intelligence.h"
#include "structs/game_object.h"

s32 func_00267D94(CInstanceHeader *arg0)
{
  int new_var;
  CIntelligence *temp_a0;
  temp_a0 = arg0->m_pIntelligence;
  new_var = 1;
  if (temp_a0->m_Type != new_var)
  {
    if (new_var || temp_a0)
    {
      return 0;
    }
    else
    {
      return 0;
    }
  }
  return temp_a0->m_dwTypeFlags & 1;
}
