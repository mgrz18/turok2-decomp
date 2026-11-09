#include "common.h"
#include "structs/list.h"
#include "protos/list.h"
#include "structs/swoosh.h"

/* Frees every ground decal (gEngineApp_m_SwooshSystem, not the particle system) that belongs to arg1. */
void SwooshSet_FreeOwner(CSwooshSystem *arg0, sptr arg1)
{
  s32 var_s4;
  CList *temp_s2;
  SwooshDecal *temp_s1;
  SwooshDecal *var_s0;
  if (arg0->m_FreeDecals.m_nCount != 0x64)
  {
    for (var_s4 = 0; var_s4 < 0xA; var_s4++)
    {
      temp_s2 = &arg0->m_DecalLists[var_s4];
      var_s0 = temp_s2->m_pHead;
      while (var_s0 != 0)
      {
        temp_s1 = var_s0->next;
        if (var_s0->owner == arg1)
        {
          CList__Remove(temp_s2, var_s0);
          CList__AddTail(&arg0->m_FreeDecals, var_s0);
        }
        var_s0 = temp_s1;
      }
    }
  }
}
