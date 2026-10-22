/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00277CC4.c, func_00277CC4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct Box3 {
                Vec3f mn;
                Vec3f mx;
} Box3;
typedef struct WRec {
                u8 pad0[0xA0];
                Box3 box;
                u8 padB8[0x2];
                u8 bBA;
                u8 padBB[0x2];
                u8 bBD;
                u8 padBE[0x2];
} WRec;
typedef struct WTab {
                u8 pad0[0x4];
                s32 count;
                WRec r[1];
} WTab;
typedef struct M5Cell { f32 x0, z0, x1, z1; } M5Cell;
typedef struct M5Ent { s32 *a; M5Cell *cell; } M5Ent; /* 8 bytes in Turok 2 (12 in Rage Wars) */
typedef struct M5Grid {
    u8 pad0[0x14A8];
    s32 n;          /* 0x14A8 */
    M5Ent ent[1];   /* 0x14AC */
} M5Grid;
extern M5Grid D_800F7078;
void *func_002017D4();
s32 func_002132B8(Box3 *q, WRec **out, s32 *cnt)
{
  s32 hashes[64];
  M5Ent *list[64];
  s32 spare[64];
  s32 n;
  s32 ncell;
  s32 i;
  s32 ret;
  s32 j;
  s32 k;
  s32 sum;
  s32 dup;
  s32 m;
  WTab *tab;
  WRec *rec;
  register M5Cell *c asm("$3");
  s32 *wp;
  s32 off;
  M5Ent *e;
  M5Grid *g;
  g = &D_800F7078;
  n = 0;
  ncell = 0;
  i = 0;
  ret = 0;
  if (g->n > 0)
  {
    off = 0x14AC;
    do
    {
      e = (M5Ent *) (((u8 *) g) + off);
      c = e->cell;
      if ((((c->x1 > q->mn.x) && (c->x0 < q->mx.x)) && (c->z1 > q->mn.z)) && (c->z0 < q->mx.z))
      {
        list[ncell] = e;
        ncell++;
      }
      off += 8;
      i++;
    }
    while (i < g->n);
  }
  if (ncell == 1)
  {
    tab = func_002017D4(*list[0]->a, 2);
    j = tab->count;
    j--;
    rec = tab->r;
    for (; j != (-1); j--)
    {
      if ((((((q->mx.x > rec->box.mn.x) && (q->mn.x < rec->box.mx.x)) && (q->mx.z > rec->box.mn.z)) && (q->mn.z < rec->box.mx.z)) && (q->mx.y > rec->box.mn.y)) && (q->mn.y < rec->box.mx.y))
      {
        out[n] = rec;
        n++;
        if (rec->bBA != 0)
        {
          ret = 1;
        }
        if (n == 0x40)
        {
          if (1)
          {
            break;
          }
        }
      }
      rec++;
    }
    *cnt = n;
  }
  else
    if (ncell >= 2)
  {
    for (i = 0; i < ncell; i++)
    {
      tab = func_002017D4(*list[i]->a, 2);
      j = tab->count;
      j--;
      rec = tab->r;
      for (; j != (-1); j--)
      {
        if ((((((q->mx.x > rec->box.mn.x) && (q->mn.x < rec->box.mx.x)) && (q->mx.z > rec->box.mn.z)) && (q->mn.z < rec->box.mx.z)) && (q->mx.y > rec->box.mn.y)) && (q->mn.y < rec->box.mx.y))
        {
          sum = 0;
          {
            register s32 kk asm("$3");
            s32 *wq;
            kk = 0;
            wq = (s32 *) (((u8 *) rec) + 0x60);
            do { sum += *wq; wq++; kk++; } while (kk < 16);
          }
          dup = 0;
          m = i != 0;
          if (m)
          {
            for (m = 0; m < n; m++)
            {
              if (hashes[m] == sum)
              {
                dup = 1;
                break;
              }
            }
          }
          if (dup == 0)
          {
            out[n] = rec;
            hashes[n] = sum;
            n++;
            if (rec->bBA != 0)
            {
              ret = 1;
            }
            if (n == 0x40)
            {
              *cnt = n;
              return ret;
            }
          }
        }
        rec++;
      }
    }
    do
    {
      *cnt = n;
    }
    while (0);
  }
  else
  {
    *cnt = n;
  }
  return ret;
}
