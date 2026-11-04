#include "common.h"

typedef struct IntrusiveList {
    /* 0x00 */ void *head;
    /* 0x04 */ void *tail;
    /* 0x08 */ s32 prevOffset;
    /* 0x0C */ s32 nextOffset;
    /* 0x10 */ s32 count;
} IntrusiveList;

typedef struct Particle {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct Particle *next;
    /* 0x08 */ u8 pad8[0x48 - 0x8];
    /* 0x48 */ s32 owner;
} Particle;

typedef struct {
    /* 0x0000 */ u8 pad0[0x94D8];
    /* 0x94D8 */ IntrusiveList active[10];
    /* 0x95A0 */ IntrusiveList freeList;
} ParticlePool;

s32 CList__AddTail(void *, void *);
s32 CList__Remove(void *, void *);

/* Frees every active particle that belongs to arg1. */
void func_002365F8(ParticlePool *arg0, s32 arg1)
{
  s32 var_s4;
  IntrusiveList *temp_s2;
  Particle *temp_s1;
  Particle *var_s0;
  if (arg0->freeList.count != 0x64)
  {
    for (var_s4 = 0; var_s4 < 0xA; var_s4++)
    {
      temp_s2 = &arg0->active[var_s4];
      var_s0 = temp_s2->head;
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
