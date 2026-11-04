typedef signed char s8;
typedef signed int s32;

/* The actor definition (header.m_pIntelligence). */
typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ s8 pad4[0x14 - 0x4];
    /* 0x14 */ s32 flags;
} ActorDef;

typedef struct {
    /* 0x00 */ s8 pad0[0x14];
    /* 0x14 */ ActorDef *def;
} Instance;

s32 func_00267D94(Instance *arg0)
{
  int new_var;
  ActorDef *temp_a0;
  temp_a0 = arg0->def;
  new_var = 1;
  if (temp_a0->kind != new_var)
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
  return temp_a0->flags & 1;
}
