#include "common.h"

typedef struct Bounds {
    f32 min[3];             /* 0x00 */
    f32 max[3];             /* 0x0C */
} Bounds;

typedef struct Rect {
    f32 minX, minZ;         /* 0x00 */
    f32 maxX, maxZ;         /* 0x08 */
} Rect;

typedef struct Region {
    void **data;            /* 0x00 */
    Rect *rect;             /* 0x04 */
} Region;

typedef struct World {
    u8 pad0[0x14A8];
    s32 regionCount;        /* 0x14A8 */
    Region regions[1];      /* 0x14AC */
} World;

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

void *func_002017D4(void *table, s32 index);

/* Collects up to 10 items of the region containing pos whose bounds hold it. */
void func_002228E4(World *world, f32 *pos, Item **out, s32 *outCount) {
    Region *found;
    ItemList *list;
    Item *item;
    Item *items;
    s32 i;
    s32 count;
    s32 n;
    s32 regionCount;
    Bounds *b;
    Rect *r;
    f32 x, z;

    found = NULL;
    count = 0;
    x = pos[0];
    z = pos[2];
    regionCount = world->regionCount;
    for (i = 0; i < regionCount; i++) {
        r = world->regions[i].rect;
        if (r->maxX < x || x < r->minX || r->maxZ < z || z < r->minZ) {
            continue;
        }
        found = &world->regions[i];
        break;
    }
    if (found == NULL) {
        *outCount = 0;
        return;
    }
    list = func_002017D4(*found->data, 2);
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
