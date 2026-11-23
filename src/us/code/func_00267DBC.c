#include "common.h"
#include "structs/intelligence.h"
#include "structs/game_object.h"

/* The instance's intelligence is type 1 with type flag 2 (GameObject_IsLeader: flag 1). */
s32 GameObject_IsFollower(CInstanceHeader *arg0)
{
  CIntelligence *temp_a0;
  unsigned char new_var;
  temp_a0 = arg0->m_pIntelligence;
  new_var = 0;
  if ((*((s32 *) (((s8 *) temp_a0) + new_var))) != 1)
  {
    if (temp_a0)
    {
      return new_var;
    }
    else
    {
      return 0;
    }
  }
  return temp_a0->m_dwTypeFlags & 2;
}
