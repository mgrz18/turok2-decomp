#include "structs/game_object.h"

s32 func_00268080(CGameObjectInstance *actor)
{
  u32 *flags;
  CIntelligence *def;
  def = actor->header.m_pIntelligence;
  if (def->m_Type != INTELLIGENCE_ENEMY)
  {
    if (def || flags)
    {
      return 0;
    }
    else
    {
      return 0;
    }
  }
  flags = &def->m_Enemy.m_dwEnemyFlags;
  return *flags & 0x20;
}
