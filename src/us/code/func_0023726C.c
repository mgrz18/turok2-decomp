/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002661FC.c, func_002661FC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct B15Def {
               s32 type;
               u8 pad4[0xA];
               s8 unkE;
} B15Def;
typedef struct B15Obj B15Obj;
struct B15Obj {               /* Turok 2 layout (Rage Wars offsets in comments) */
                u8 type;
                u8 pad1;
                u16 unk4;             /* 0x002 (0x004) */
                u8 pad4[0x10];
                B15Def *def;          /* 0x014 (0x018) */
                u8 pad18[0xA0];
                u16 unkE4;            /* 0x0B8 (0x0E4): type id */
                u8 padBA[0x1A];
                u32 flags;            /* 0x0D4 (0x100) */
                u8 padD8[0x20];
                s32 *unk118;          /* 0x0F8 (0x118): parent */
                u8 padFC[0x10];
                B15Obj *unk12C;       /* 0x10C (0x12C) */
};
s32 func_00267090(B15Obj *a);
extern s32 D_8011001C;
extern u8 D_8011AAF1;
static __inline__ s32 chk(B15Obj *a, B15Obj *t) {
    u32 f;
    if (D_8011AAF1 != 0) {
        return 1;
    }
    if (t->unkE4 == a->unkE4) {
        return 0;
    }
    f = a->flags;
    if (!(f & 0x310000)) {
        return 0;
    }
    if (D_8011001C != 0 && !(t->flags & 0x300000) && !(f & 0x300000)) {
        return 0;
    }
    switch (a->def->type) {
    case 1:
    case 4:
    case 7:
    case 0xB:
        return 1;
    }
    return 0;
}
static __inline__ s32 chk3(B15Obj *a, B15Obj *t) {
    u32 f;
    register s32 res asm("$3");
    if (D_8011AAF1 != 0) {
        res = 1;
        goto done;
    }
    if (t->unkE4 == a->unkE4) {
        res = 0;
        goto done;
    }
    f = a->flags;
    if (!(f & 0x310000)) {
        res = 0;
        goto done;
    }
    if (D_8011001C != 0 && !(t->flags & 0x300000) && !(f & 0x300000)) {
        res = 0;
        goto done;
    }
    switch (a->def->type) {
    case 1:
    case 4:
    case 7:
    case 0xB:
        res = 1;
        goto done;
    }
    res = 0;
done:
    return res;
}
s32 func_0023726C(B15Obj *a, B15Obj *t)
{
  B15Obj *p;
  B15Obj *u;
  if (a == t)
  {
    return 0;
  }
  if ((a->flags & 0x300000) && (!func_00267090(a)))
  {
    return 0;
  }
  if (t->type != 1)
  {
    if (t->type != 2)
    {
      return 1;
    }
    p = t->unk12C;
    u = t;
    if (p != ((void *) 0))
    {
      if ((p->type == 1) && (!chk(a, p)))
      {
        return 0;
      }
      if ((a == u->unk12C) && ((*u->unk118) & 0x40))
      {
        return 0;
      }
    }
    if (((u->unk4 != 0x14) || (a->def->type != 1)) || (a->unkE4 != 0xCA))
    {
      return 1;
    }
    return 0;
  }
  if (chk3(a, t))
  {
    return 1;
  }
  if (a || t)
  {
 do { } while (0);
    return 0;
  }
  else
  {
    return 0;
  }
}
