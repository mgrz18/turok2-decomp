/* The do { } while (0) around the region-loop setup raises the region counter's priority (GCC weights refs by loop depth) so it gets fp and the object-table base is spilled, as in the ROM. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00289054.c, func_00289054, with Turok 2's layouts. */
#include "common.h"
#include "structs/game.h"
#include "structs/player.h"
#include "gfx.h"
#include "structs/world.h"
#include "structs/level_actor.h"
#include "structs/camera.h"
#include "structs/rect.h"

/* Draws the level geometry seen by one view: every static object and level
 * actor of the grid regions inside the view's box that is shown in this view
 * and inside its frustum is drawn, or queued on the world's sorted list
 * (translucent ones, drawn later by func_0022077C). Objects flagged 8 also
 * go on a second list, except the type found at 0x33 in the world's id table
 * unless the view's player passes the func_00253720(0x111F) test. With the
 * shared flag set, the cached list of func_00221D3C is used instead. */

typedef struct {
    s32 stride;             /* 0x00 */
    s32 count;              /* 0x04 */
    u8 entries[1];          /* 0x08 */
} Table;

typedef struct {
    s32 stride;             /* 0x00 */
    s32 count;              /* 0x04 */
    u16 ids[1];             /* 0x08 */
} IdTable;

extern u8 D_8011AAF1;       /* D_8011AAD8.multiplayer */
extern s32 D_800B6D20;

void func_00221D3C(CWorld *);
CPlayer *func_00281194(CCamera *);
s32 func_00253720(CPlayer *, s32);
void func_0020B5D4(void);
void func_0020B624(void);
void func_0020565C(s32, void **);
void *func_002017D4(void *, s32);
s32 func_0026F60C(void *, CBox *);
void func_00214D04(CStaticObject *, CCamera *);
void func_00214844(CStaticObject *, CCamera *);
void func_002152AC(CLevelActor *);

void func_002222E0(CWorld *w, CCamera *v_) {
    register CCamera *v asm("$18") = v_;
    u8 *base;
    void *grp;
    s32 i;
    s32 n;
    s32 mask;
    s32 vmask;
    s32 idx;
    s32 flag;
    CPlayer *player;
    IdTable *tbl;
    s32 cnt;
    s32 si;
    u16 *ip;
    Table *t;
    CWorldSector *regions;
    CWorldSector *region;
    CRect *cell;
    CStaticObject *a;
    CLevelActor *p;
    CLevelActor *actors;
    s32 k;
    s32 cnt1;
    s32 cnt2;
    s32 off;
    s32 c;

    mask = D_8011AAD8.difficulty;
    vmask = v->m_ViewParams.m_VisBits;
    if (D_8011AAD8.multiplayer != 0) {
        func_00221D3C(w);
        return;
    }
    player = func_00281194(v);
    si = 0;
    tbl = (IdTable *)w->m_pObjectTypes;
    cnt = tbl->count;
    ip = tbl->ids;
    if (cnt > 0) {
        do {
            if (ip[si] == 0x33) {
                idx = si;
                goto found;
            }
            si++;
        } while (si < cnt);
    }
    idx = -1;
found:
    if (player != NULL && idx != -1 && func_00253720(player, 0x111F) != 0 && D_8011AAF1 == 0) {
        flag = 1;
    } else {
        flag = 0;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE7000000;
        g->w1 = 0;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE3000A01;
        g->w1 = 0x100000;
    }
    func_0020B5D4();
    w->m_nDeferred = 0;
    if (player != NULL && (player->m_ModeFlags & 0x100)) {
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB040004;
            g->w1 = 1;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB04000C;
            g->w1 = 1;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB040014;
            g->w1 = 0xFFFF;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB04001C;
            g->w1 = 0xFFFF;
        }
    } else {
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB040004;
            g->w1 = 2;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB04000C;
            g->w1 = 2;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB040014;
            g->w1 = 0xFFFE;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDB04001C;
            g->w1 = 0xFFFE;
        }
    }
    do {
        i = 0;
        n = w->m_nSectors;
        regions = w->m_Sectors;
    } while (0);
    if (n > 0) {
        do {
            region = regions + i;
            cell = region->m_pBounds;
            if (cell->maxX > v->m_ViewMin.x && cell->minX < v->m_ViewMax.x
                && cell->maxZ > v->m_ViewMin.z && cell->minZ < v->m_ViewMax.z) {
                func_0020565C(0, region->m_hData);
                k = 0;
                grp = *region->m_hData;
                t = func_002017D4(grp, k);
                cnt1 = t->count;
                base = t->entries;
                if (cnt1 > 0) {
                    off = k;
                    do {
                        a = (CStaticObject *)(base + off);
                        if (a->header.m_pIntelligence->m_dwFlags & mask) {
                            a->m_pOwner = region->m_hData;
                            if (D_800B6D20 == 0) {
                                func_00214D04(a, v);
                            }
                            if ((vmask & a->m_ViewMask)
                                && v->m_ViewMax.x > a->m_Bounds.m_vMin.x && v->m_ViewMin.x < a->m_Bounds.m_vMax.x
                                && v->m_ViewMax.z > a->m_Bounds.m_vMin.z && v->m_ViewMin.z < a->m_Bounds.m_vMax.z
                                && v->m_ViewMax.y > a->m_Bounds.m_vMin.y && v->m_ViewMin.y < a->m_Bounds.m_vMax.y
                                && func_0026F60C(v->m_ViewVolume, &a->m_Bounds) != 0) {
                                if ((a->m_wFlags & 8) && (a->header.m_nObjType != idx || flag != 0)) {
                                    c = w->m_nListed;
                                    if (c != 0x40) {
                                        w->m_pListed[c] = (CGameObjectInstance *)a;
                                        w->m_nListed = c + 1;
                                    }
                                }
                                if (a->m_wFlags & 4) {
                                    c = w->m_nDeferred;
                                    if (c != 0x40) {
                                        w->m_pDeferred[c] = a;
                                        w->m_nDeferred = c + 1;
                                    }
                                } else {
                                    func_00214844(a, v);
                                }
                            }
                        }
                        k++;
                        off += sizeof(CStaticObject);
                    } while (k < cnt1);
                }
                t = func_002017D4(grp, 1);
                k = 0;
                cnt2 = t->count;
                actors = (CLevelActor *)t->entries;
                if (cnt2 > 0) {
                    do {
                        p = actors + k;
                        if (p->header.m_pIntelligence->m_dwFlags & mask) {
                            u16 fl = p->m_wFlags;

                            if (!(fl & 1) && (vmask & p->m_ViewMask)
                                && v->m_ViewMax.x > p->m_Bounds.m_vMin.x && v->m_ViewMin.x < p->m_Bounds.m_vMax.x
                                && v->m_ViewMax.z > p->m_Bounds.m_vMin.z && v->m_ViewMin.z < p->m_Bounds.m_vMax.z
                                && v->m_ViewMax.y > p->m_Bounds.m_vMin.y && v->m_ViewMin.y < p->m_Bounds.m_vMax.y
                                && (fl & 2) && func_0026F60C(v->m_ViewVolume, &p->m_Bounds) != 0) {
                                if (p->m_wFlags & 0x20) {
                                    c = w->m_nDeferred;
                                    if (c != 0x40) {
                                        w->m_pDeferred[c] = p;
                                        w->m_nDeferred = c + 1;
                                    }
                                } else {
                                    func_002152AC(p);
                                }
                            }
                        }
                        k++;
                    } while (k < cnt2);
                }
            }
            i++;
        } while (i < n);
    }
    func_0020B624();
}
