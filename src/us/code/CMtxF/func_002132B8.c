/* FAKEMATCH (round 21 audit): 2 register pins (`register ... asm("$n")`); without them:
 * Lighting_CollectVolumes 76 of 243 words (size 0x3c8 vs 0x3cc). */
#include "structs/vector.h"
#include "globals/scene.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00277CC4.c, func_00277CC4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"
#include "structs/light.h"
typedef struct M5Cell { f32 x0, z0, x1, z1; } M5Cell;
s32 Lighting_CollectVolumes(CBox *q, CLightVolume **out, s32 *cnt)
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
  CLightVolumeTable *tab;
  CLightVolume *rec;
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
      e = &g->m_Sectors[i]; /* N64 0x14AC + 8 * i: the host CSceneSector is two pointers */
      c = e->m_pBounds;
      if ((((c->x1 > q->m_vMin.x) && (c->x0 < q->m_vMax.x)) && (c->z1 > q->m_vMin.z)) && (c->z0 < q->m_vMax.z))
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
    rec = tab->volumes;
    for (; j != (-1); j--)
    {
      if ((((((q->m_vMax.x > rec->m_Bounds.m_vMin.x) && (q->m_vMin.x < rec->m_Bounds.m_vMax.x)) && (q->m_vMax.z > rec->m_Bounds.m_vMin.z)) && (q->m_vMin.z < rec->m_Bounds.m_vMax.z)) && (q->m_vMax.y > rec->m_Bounds.m_vMin.y)) && (q->m_vMin.y < rec->m_Bounds.m_vMax.y))
      {
        out[n] = rec;
        n++;
        if (rec->unkBA != 0)
        {
          ret = 1;
        }
        if (n == 0x40)
        {
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
      rec = tab->volumes;
      for (; j != (-1); j--)
      {
        if ((((((q->m_vMax.x > rec->m_Bounds.m_vMin.x) && (q->m_vMin.x < rec->m_Bounds.m_vMax.x)) && (q->m_vMax.z > rec->m_Bounds.m_vMin.z)) && (q->m_vMin.z < rec->m_Bounds.m_vMax.z)) && (q->m_vMax.y > rec->m_Bounds.m_vMin.y)) && (q->m_vMin.y < rec->m_Bounds.m_vMax.y))
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
            if (rec->unkBA != 0)
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
    *cnt = n;
  }
  else
  {
    *cnt = n;
  }
  return ret;
}
