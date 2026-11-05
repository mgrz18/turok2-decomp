#include "common.h"
#include "structs/intelligence.h"
#include "structs/game_object.h"

s32 func_002680A8(CInstanceHeader *arg0)
{
  CIntelligence *new_var2;
  unsigned int new_var3;
  CIntelligence *temp_a0;
  CIntelligence **new_var;
  new_var = &arg0->m_pIntelligence;
  temp_a0 = *new_var;
  if ((new_var2 = temp_a0)->m_Type != 1)
  {
    return 0;
 new_var3 = 0; do { } while (new_var3);
  }
  return new_var2->m_Enemy.m_dwEnemyFlags & 0x200;
}
