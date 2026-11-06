#include "common.h"
#include "structs/world.h"
#include "structs/collision.h"
#include "structs/rect.h"

typedef struct GridCell {
    CRect rect;                  /* 0x00 */
    s32 empty;                  /* 0x10 */
} GridCell;                     /* 0x14 bytes */

typedef struct FloatList {
    s32 pad0;
    s32 count;                  /* 0x04 */
    f32 values[1];              /* 0x08 */
} FloatList;

typedef struct Viewer {
    u8 pad0[0x4];
    struct Viewer *next;        /* 0x004 */
    u8 pad8[0x350 - 0x8];
    f32 min[3];                 /* 0x350 */
    f32 max[3];                 /* 0x35C */
} Viewer;

extern Viewer *gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead;      /* viewers */
extern CBox gCameraVisibleBox;       /* visible bounds: min xyz, max xyz */
extern char D_800A5F6C[];       /* "grid section" */

void *CISet__GetBlock(void *table, s32 index);
void *CISet__GetROMAddressAndSize(void *file, s32 arg1, s32 index, s32 *size);
void **CCache__Request(s32 arg0, void *data, s32 size, s32 align, void *owner, s32 (*load)(), char *name, s32 arg7);
s32 CScene__DecompressGridSection();
void CCache__Unlock(s32 arg0, void **handle);

/* Loads the grid sections inside the visible bounds that some viewer overlaps. */
void CScene__LoadVisibleGridSections(CScene *world) {
    s32 size;
    s32 nx;
    s32 x0;
    GridCell *cells;
    Viewer *viewers;
    Viewer *v;
    CSceneSector *region;
    GridCell *cell;
    void **handle;
    f32 *xMax;
    f32 *xMin;
    f32 *zMax;
    f32 *zMin;
    FloatList *list;
    s32 nz;
    s32 x1;
    s32 z0;
    s32 z1;
    s32 x;
    s32 z;
    s32 idx;
    s32 found;
    s32 count;

    viewers = gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead;
    if (viewers == NULL) {
        return;
    }
    x0 = 0;
    count = 0;
    list = CISet__GetBlock(world->m_pCollision, 0);
    xMax = list->values;
    nx = list->count;
    xMin = ((FloatList *)CISet__GetBlock(world->m_pCollision, 1))->values;
    x1 = nx - 1;
    list = CISet__GetBlock(world->m_pCollision, 2);
    nz = list->count;
    zMax = list->values;
    zMin = ((FloatList *)CISet__GetBlock(world->m_pCollision, 3))->values;
    cells = (GridCell *)((u8 *)CISet__GetBlock(world->m_pCollision, 4) + 8);
    z1 = nz - 1;
    z0 = 0;
    while (x0 < nx && xMin[x0] < gCameraVisibleBox.m_vMin.x) {
        x0++;
    }
    while (x1 >= 0 && gCameraVisibleBox.m_vMax.x < xMax[x1]) {
        x1--;
    }
    while (z0 < nz && zMin[z0] < gCameraVisibleBox.m_vMin.z) {
        z0++;
    }
    while (z1 >= 0 && gCameraVisibleBox.m_vMax.z < zMax[z1]) {
        z1--;
    }
    region = world->m_Sectors;
    for (z = z0; z <= z1; z++) {
        for (x = x0; x <= x1; x++) {
            idx = x + z * nx;
            cell = &cells[idx];
            if (cell->empty != 0) {
                continue;
            }
            found = 0;
            for (v = viewers; v != NULL; v = v->next) {
                if (v->min[0] < cell->rect.maxX && cell->rect.minX < v->max[0]
                    && cell->rect.maxZ > v->min[2] && cell->rect.minZ < v->max[2]) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                continue;
            }
            handle = CCache__Request(0, CISet__GetROMAddressAndSize(world->m_pLevelInfo, world->m_LevelInfoKey, idx, &size), size, 8, world,
                                   CScene__DecompressGridSection, D_800A5F6C, 0);
            if (handle == NULL) {
                continue;
            }
            if (*(s32 *)*handle != 0) {
                region->m_hData = handle;
                region->m_pBounds = cell;
                region++;
                if (++count == 128) {
                    goto done;
                }
            } else {
                CCache__Unlock(0, handle);
            }
        }
    }
done:
    world->m_nSectors = count;
}
