#include "common.h"
#include "structs/world.h"
#include "structs/rect.h"

typedef struct Bounds {
    f32 min[3];             /* 0x00 */
    f32 max[3];             /* 0x0C */
} Bounds;

typedef struct Item {
    u8 pad0[0xA0];
    Bounds bounds;          /* 0xA0 */
    u8 padB8[0xC0 - 0xB8];
} Item;                     /* 0xC0 bytes */

typedef struct ItemList {
    s32 pad0;
    s32 count;              /* 0x04 */
    Item items[1];          /* 0x08 */
} ItemList;

void *CISet__GetBlock(void *table, s32 index);

/* Collects up to 10 items of the region containing pos whose bounds hold it. */
void CScene__FindLightsAt(CScene *world, f32 *pos, Item **out, s32 *outCount) {
    CSceneSector *found;
    ItemList *list;
    Item *item;
    Item *items;
    s32 i;
    s32 count;
    s32 n;
    s32 regionCount;
    Bounds *b;
    CRect *r;
    f32 x, z;

    found = NULL;
    count = 0;
    x = pos[0];
    z = pos[2];
    regionCount = world->m_nSectors;
    for (i = 0; i < regionCount; i++) {
        r = world->m_Sectors[i].m_pBounds;
        if (r->maxX < x || x < r->minX || r->maxZ < z || z < r->minZ) {
            continue;
        }
        found = &world->m_Sectors[i];
        break;
    }
    if (found == NULL) {
        *outCount = 0;
        return;
    }
    list = CISet__GetBlock(*found->m_hData, 2);
    n = list->count;
    items = list->items;
    for (i = 0; i < n; i++) {
        item = &items[i];
        b = &item->bounds;
        if (b->max[0] > pos[0] && b->max[1] > pos[1] && b->max[2] > pos[2]
            && b->min[0] <= pos[0] && b->min[1] <= pos[1] && b->min[2] <= pos[2]) {
            out[count++] = item;
            if (count == 10) {
                break;
            }
        }
    }
    *outCount = count;
}
