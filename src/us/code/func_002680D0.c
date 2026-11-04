#include "structs/game_object.h"

s32 func_002680D0(CGameObjectInstance *actor)
{
  int new_var;
  CIntelligence *def;
  def = actor->header.m_pIntelligence;
  if (def->m_Type != ((unsigned long long) INTELLIGENCE_ENEMY))
  {
    new_var = 0;
    return new_var;
 do { } while (new_var);
  }
  return def->m_Enemy.m_dwEnemyFlags & 0x400;
}
