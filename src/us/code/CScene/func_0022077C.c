#include "common.h"
#include "gfx.h"
#include "structs/vector.h"

#ifdef TARGET_PORT
#include "structs/world.h"
#include "structs/level_actor.h"
/* The port: the list is CScene.m_nDeferred / m_pDeferred (the caller passes the world). The N64
 * reads them at 0x1064 / 0x1068; on the host those offsets fall elsewhere in CScene, so no
 * translucent (flag 4) instance or static object was ever drawn (the Iguana cinema's iguanas:
 * round 9 x3, docs/port/FIDELITY.md). An item is a CStaticObject (type 0: its box) or an
 * instance (its header position); the views at the host offsets. */
typedef struct Drawable {
    union {
        struct { u8 type; };                                   /* 0x00: 0 box, 1 point, 3 ... */
        struct { u8 _hp_pos[__builtin_offsetof(CInstanceHeader, m_vPos)]; CVector3 pos; }; /* N64 0x04 */
        struct { u8 _hp_min[__builtin_offsetof(CStaticObject, m_Bounds.m_vMin)]; CVector3 min; }; /* N64 0x68 */
        struct { u8 _hp_max[__builtin_offsetof(CStaticObject, m_Bounds.m_vMax)]; CVector3 max; }; /* N64 0x74 */
    };
} Drawable;

typedef struct SortEntry {
    Drawable *item;             /* 0x00 */
    f32 dist;                   /* 0x04: squared, doubled */
} SortEntry;

typedef struct DrawList {
    union {
        struct { u8 _hp_count[__builtin_offsetof(CScene, m_nDeferred)]; s32 count; }; /* N64 0x1064 */
        struct { u8 _hp_items[__builtin_offsetof(CScene, m_pDeferred)]; Drawable *items[0x40]; }; /* N64 0x1068 */
    };
} DrawList;
#else
typedef struct Drawable {
    u8 type;                    /* 0x00: 0 box, 1 point, 3 ... */
    u8 pad1[3];
    CVector3 pos;                   /* 0x04 */
    u8 pad10[0x68 - 0x10];
    CVector3 min;                   /* 0x68 */
    CVector3 max;                   /* 0x74 */
} Drawable;

typedef struct SortEntry {
    Drawable *item;             /* 0x00 */
    f32 dist;                   /* 0x04: squared, doubled */
} SortEntry;

typedef struct DrawList {
    u8 pad0[0x1064];
    s32 count;                  /* 0x1064 */
    Drawable *items[1];         /* 0x1068 */
} DrawList;
#endif

CVector3 CCamera__GetViewOrigin(void *cam);  /* camera position */
void Sort_Quick(void *base, s32 count, s32 size, s32 (*cmp)(), s32 (*swap)());
s32 SortRecord_Compare();
s32 SortRecord_Swap();
void Render_ResetBatches(void);
void Render_FlushBatches(void);
void CStaticObject__Draw(Drawable *item, void *cam);
void CGameObjectInstance__Draw(Drawable *item, void *cam);
void CLevelActor__Draw(Drawable *item);

/* Sorts the list by distance from the camera, then draws it. */
void CScene__DrawSortedList(DrawList *list, void *cam) {
    SortEntry entries[0x800];
    s32 count;
    s32 i;
    f32 cx, cy, cz;
    f32 dx, dy, dz;
    Drawable *item;

    count = list->count;
    if (count == 0) {
        return;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xDB040004;
        g->w1 = 1;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xDB04000C;
        g->w1 = 1;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xDB040014;
        g->w1 = 0xFFFF;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xDB04001C;
        g->w1 = 0xFFFF;
    }
    cx = CCamera__GetViewOrigin(cam).x * -2.0f;
    cy = CCamera__GetViewOrigin(cam).y * -2.0f;
    cz = CCamera__GetViewOrigin(cam).z * -2.0f;
    for (i = 0; i < count; i++) {
        item = list->items[i];
        switch (item->type) {
        case 0:
            dx = item->min.x + item->max.x + cx;
            dy = item->min.y + item->max.y + cy;
            dz = item->min.z + item->max.z + cz;
            break;
        case 1:
            dx = item->pos.x * 2.0f + cx;
            dy = item->pos.y * 2.0f + cy;
            dz = item->pos.z * 2.0f + cz;
            break;
        case 3:
        default:
            dx = item->pos.x * 2.0f + cx;
            dy = item->pos.y * 2.0f + cy;
            dz = item->pos.z * 2.0f + cz;
            break;
        }
        entries[i].item = item;
        entries[i].dist = dx * dx + dy * dy + dz * dz;
    }
    Sort_Quick(entries, count, sizeof(SortEntry), SortRecord_Compare, SortRecord_Swap);
    Render_ResetBatches();
    for (i = 0; i < count; i++) {
        item = entries[i].item;
        switch (item->type) {
        case 0:
            CStaticObject__Draw(item, cam);
            break;
        case 3:
            CLevelActor__Draw(item);
            break;
        case 1:
            CGameObjectInstance__Draw(item, cam);
            break;
        }
    }
    Render_FlushBatches();
}
