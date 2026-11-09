#include "structs/vector.h"
#include "globals/scene.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00277CC4.c, func_00277CC4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"
typedef struct Box3 {
                CVector3 mn;
                CVector3 mx;
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
s32 Lighting_CollectVolumes(Box3 *q, WRec **out, s32 *cnt)
{
  s32 hashes[64];
  CSceneSector *list[64];
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
  CSceneSector *e;
  CScene *g;
  g = &gScene;
  n = 0;
  ncell = 0;
  i = 0;
  ret = 0;
  if (g->m_nSectors > 0)
  {
    off = 0x14AC;
    do
    {
      e = (CSceneSector *) (((u8 *) g) + off);
      c = e->m_pBounds;
      if ((((c->x1 > q->mn.x) && (c->x0 < q->mx.x)) && (c->z1 > q->mn.z)) && (c->z0 < q->mx.z))
      {
        list[ncell] = e;
        ncell++;
      }
      off += 8;
      i++;
    }
    while (i < g->m_nSectors);
  }
  if (ncell == 1)
  {
    tab = CISet__GetBlock(*list[0]->m_hData, 2);
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
      tab = CISet__GetBlock(*list[i]->m_hData, 2);
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
