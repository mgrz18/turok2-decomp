#include "structs/game_object.h"

f32 func_00268534(CGameObjectInstance *actor)
{
  CIntelligence *def;
  f32 new_var;
  f32 new_var2;
  def = actor->header.m_pIntelligence;
  if (def->m_Type != INTELLIGENCE_ENEMY)
  {
    return 0.0f;
  }
  new_var2 = def->m_Enemy.m_LeashRadius;
  if (new_var)
  {
    new_var = new_var2;
    return new_var;
  }
  else
  {
    new_var = new_var2;
    return new_var;
  }
}
