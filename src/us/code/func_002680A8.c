#include "common.h"
#include "structs/intelligence.h"

typedef struct {
    /* 0x00 */ s8 pad0[0x14];
    /* 0x14 */ CIntelligence *def;
} Instance;

s32 func_002680A8(Instance *arg0)
{
  CIntelligence *new_var2;
  unsigned int new_var3;
  CIntelligence *temp_a0;
  CIntelligence **new_var;
  new_var = &arg0->def;
  temp_a0 = *new_var;
  if ((new_var2 = temp_a0)->m_Type != 1)
  {
    return 0;
 new_var3 = 0; do { } while (new_var3);
  }
  return new_var2->m_Enemy.m_dwEnemyFlags & 0x200;
}
