/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00440F10.c, func_00440F10.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct { s16 type; u8 pad2[2]; u32 flags; u16 w8; u16 wA; u8 bC, bD, bE, bF; s32 f10; u8 pad14[0x24 - 0x14]; } Q04Tpl;
typedef struct { void *unk0; s16 n; u8 pad6[2]; void (*unk8)(void *, void *); u8 pad0C[0x18 - 0xC]; u16 w18; u16 w1A; u16 w1C; } Q04Hdr;
typedef struct { u8 pad[4]; u32 flags; u8 pad8[0x1C - 8]; void (*unk1C)(void *, void *, void *); } Q04ItemDef;
typedef struct { s32 idx; u16 w4; u8 pad6[2]; u32 flags; u16 wC; u16 wE; u8 b10, b11, b12, b13; s32 f14; Q04ItemDef *def; s32 f1C; void *p20; void *owner; } Q04Itm;
typedef struct { u16 a; u8 pad2[2]; u16 b; u8 pad6[2]; u16 c; u8 padA[3]; u8 d; u8 pad0E[3]; u8 e; u8 pad12[3]; u8 f; u8 pad16[2]; } Q04Tab;
typedef struct { u16 w0, w2, w4, w6, w8, wA; u8 bC, bD, bE, bF; } Q04Rec;
typedef struct {
    u16 w0, w2, w4;
    u8 pad6[2];
    void *p8;
    Q04Itm *items;
    s32 n;
    void *hdr;
    s32 f18;
    s32 f1C;
    void *f20;
    s32 f24;
    u16 w28;
    u16 w2A;
    s32 f2C, f30, f34, f38, f3C;
    s32 f40;
    void *f44;
    Q04Rec rec[2][8];
} Q04Panel;
extern s32 func_00416B28();
extern void func_00416C00();
extern void func_00416C0C(void *p);
extern Q04Tab D_800C1CD2[];
void func_0044DEA8(Q04Panel *p, void *a1, Q04Hdr *t, s32 a3, s32 a4, void *a5, s32 a6, void *a7)
{
  s32 i;
  s32 j;
  u8 new_var;
  s32 k;
  s32 k2;
  s32 off;
  u8 *e;
  s32 new_var2;
  Q04Itm *ib;
  u8 *ext;
  u8 **pc;
  p->w0 = t->w18;
  p->w2 = t->w1A;
  p->w4 = t->w1C;
  p->p8 = a1;
  ib = (Q04Itm *) (((u8 *) p) + 0x1D8);
  p->items = ib;
  p->n = t->n;
  ext = (u8 *) ib;
  ext = ((u8 *) ib) + (t->n * 0x28);
  for (i = 0; i < t->n; i++)
  {
    Q04Itm *it = (Q04Itm *) ((i * 0x28) + ((s32) p->items));
    Q04Tpl *tp = (Q04Tpl *) ((i * 0x24) + ((s32) t->unk0));
    it->idx = i;
    pc = &ext;
    {
      it->w4 = tp->type;
      it->flags = tp->flags | 0x01800000;
      it->wC = tp->w8;
      it->wE = tp->wA;
      it->b10 = tp->bC;
    }
    it->b11 = tp->bD;
    it->b12 = tp->bE;
    it->b13 = tp->bF;
    new_var2 = tp->f10;
    it->def = (Q04ItemDef *) tp;
    it->f1C = -1;
    it->owner = p;
    it->f14 = new_var2;
    k = 0;
    if (tp->type == 3)
    {
      k = 0x330; /* Turok 2 element size (Rage Wars: 0x480) */
    }
    if (k != 0)
    {
      e = *pc;
      it->p20 = e;
      if (tp->type == 3)
      {
        *((s32 *) (e + 0x328)) = 1;
      }
      k2 = 0;
      if (tp->type == 3)
      {
        k2 = 0x330;
      }
      *pc += k2;
    }
  }
  p->hdr = t;
  p->f18 = a3;
  p->f1C = a4;
  p->f20 = a5;
  p->f24 = a6;
  p->w28 = 0;
  p->w2A = *((u16 *) (((u8 *) a7) + 0x14));
  p->f2C = 0;
  p->f30 = 0;
  p->f34 = 0;
  p->f38 = 1;
  p->f3C = 0;
  p->f40 = func_00416B28(a7);
  p->f44 = a7;
  for (j = 0; j < 2; j++)
  {
    for (i = 0; i < 8; i++)
    {
      u8 *rb = ((u8 *)p) + ((j << 7) + i * 16);
      *((u16 *) (rb + 0x48)) = D_800C1CD2[i].a;
      *((u16 *) (rb + 0x4A)) = D_800C1CD2[i].b;
      *((u16 *) (rb + 0x4C)) = D_800C1CD2[i].c;
      *((u8 *) (rb + 0x54)) = D_800C1CD2[i].d;
      *((u8 *) (rb + 0x55)) = D_800C1CD2[i].e;
      new_var = D_800C1CD2[i].f;
      *((u8 *) (rb + 0x57)) = 0xFF;
      *((u16 *) (rb + 0x50)) = 0;
      *((u16 *) (rb + 0x52)) = 0;
      *((u8 *) (rb + 0x56)) = new_var;
    }
  }
  *((s32 *) (((u8 *) a5) + 0xB0)) = 0;
  *((s32 *) (((u8 *) a5) + 0xB4)) = 0;
  *((s32 *) (((u8 *) a5) + 0xBC)) = 0;
  func_00416C00(p);
  func_00416C0C(p);
  if (t->unk8 != 0)
  {
    t->unk8(p, a7);
  }
  {
    Q04Itm *it;
    i = p->n;
    it = p->items;
    i--;
    if (i != (-1))
    {
      do
      {
        Q04ItemDef *d = it->def;
        if (d->flags & 0x04000000)
        {
          if (d->unk1C != (off = 0))
          {
            d->unk1C(it, p, a7);
          }
        }
        i--;
        it++;
      }
      while (i != (-1));
    }
  }
}
