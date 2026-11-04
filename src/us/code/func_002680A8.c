typedef signed char s8;
typedef signed int s32;

/* The actor definition (header.m_pIntelligence). */
typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ s8 pad4[0x48 - 0x4];
    /* 0x48 */ s32 flags48;
} ActorDef;

typedef struct {
    /* 0x00 */ s8 pad0[0x14];
    /* 0x14 */ ActorDef *def;
} Instance;

s32 func_002680A8(Instance *arg0)
{
  ActorDef *new_var2;
  unsigned int new_var3;
  ActorDef *temp_a0;
  ActorDef **new_var;
  new_var = &arg0->def;
  temp_a0 = *new_var;
  if ((new_var2 = temp_a0)->kind != 1)
  {
    return 0;
 new_var3 = 0; do { } while (new_var3);
  }
  return new_var2->flags48 & 0x200;
}
