
#include "protos/scene.h"
#ifdef TARGET_PORT
#include "structs/world.h"

/* Host body (the Blind One fight reaches it): the actor table and its count at the N64 scene
 * offsets 0xE0/0xE8, the definition at actor + 0x14 and the N64 stride 0x288; the same walk
 * as CScene__UnmarkGeneratorsSaved (func_00225D0C) on host members: every type-3 actor
 * (a generator) gets its saved bit. */
void CScene__MarkGeneratorsSaved(void *arg0)
{
  CScene *scene = (CScene *) arg0;
  s32 i;

  for (i = 0; i < scene->m_nActors; i++)
  {
    CGameObjectInstance *rec = &scene->m_pActors[i];

    if (*(s32 *) rec->header.m_pIntelligence == 3)
    {
      CScene__SetInstanceSavedBit(arg0, rec, 1);
    }
  }
}
#else
void CScene__MarkGeneratorsSaved(void *arg0)
{
  s32 temp_s2;
  int new_var;
  s32 var_s0;
  s32 var_s1;
  void *temp_a1;
  temp_s2 = *((s32 *) (((s8 *) arg0) + 0xE8));
  var_s0 = 0;
  var_s1 = 0;
  if (temp_s2 > 0)
  {
    new_var = 3;
    var_s1 = 0;
    do
    {
      temp_a1 = (*((s32 *) (((s8 *) arg0) + 0xE0))) + var_s1;
      if ((*(*((s32 **) (((s8 *) temp_a1) + 0x14)))) == new_var)
      {
        CScene__SetInstanceSavedBit(arg0, temp_a1, 1);
      }
      var_s0 += 1;
      var_s1 += 0x288;
    }
    while (var_s0 < temp_s2);
  }
}
#endif
