#include "common.h"
#include "structs/intelligence.h"

typedef struct {
    /* 0x00 */ s8 pad0[0x14];
    /* 0x14 */ CIntelligence *def;
} Instance;

s32 func_00267D94(Instance *arg0)
{
  int new_var;
  CIntelligence *temp_a0;
  temp_a0 = arg0->def;
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
