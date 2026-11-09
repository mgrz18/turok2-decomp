#include "common.h"
#include "structs/list.h"
#include "protos/list.h"

typedef struct SwooshNode {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct SwooshNode *next;
    /* 0x08 */ u8 pad8[0x48 - 0x8];
    /* 0x48 */ s32 owner;
} SwooshNode;

typedef struct {
    /* 0x0000 */ u8 pad0[0x94D8];
    /* 0x94D8 */ CList active[10];
    /* 0x95A0 */ CList freeList;
} SwooshSet;


/* Frees every active node of the swoosh/decal set (gEngineApp_m_SwooshSystem, not the particle system) that belongs to arg1. */
void SwooshSet_FreeOwner(SwooshSet *arg0, s32 arg1)
{
  s32 var_s4;
  CList *temp_s2;
  SwooshNode *temp_s1;
  SwooshNode *var_s0;
  if (arg0->freeList.m_nCount != 0x64)
  {
    for (var_s4 = 0; var_s4 < 0xA; var_s4++)
    {
      temp_s2 = &arg0->active[var_s4];
      var_s0 = temp_s2->m_pHead;
      while (var_s0 != 0)
      {
        temp_s1 = var_s0->next;
        if (var_s0->owner == arg1)
        {
          CList__Remove(temp_s2, var_s0);
          CList__AddTail(&arg0->freeList, var_s0);
        }
        var_s0 = temp_s1;
      }
    }
  }
}
