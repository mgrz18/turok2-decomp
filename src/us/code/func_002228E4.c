#include "common.h"
#include "structs/world.h"
#include "structs/rect.h"
#include "protos/iset.h"
#include "structs/vector.h"
#include "structs/light.h"





/* Collects up to 10 items of the region containing pos whose bounds hold it. */
void CScene__FindLightsAt(CScene *world, f32 *pos, CLightVolume **out, s32 *outCount) {
    CSceneSector *found;
    CLightVolumeTable *list;
    CLightVolume *item;
    CLightVolume *items;
    s32 i;
    s32 count;
    s32 n;
    s32 regionCount;
    CBox *b;
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
    items = list->volumes;
    for (i = 0; i < n; i++) {
        item = &items[i];
        b = &item->m_Bounds;
        if (b->m_vMax.x > pos[0] && b->m_vMax.y > pos[1] && b->m_vMax.z > pos[2]
            && b->m_vMin.x <= pos[0] && b->m_vMin.y <= pos[1] && b->m_vMin.z <= pos[2]) {
            out[count++] = item;
            if (count == 10) {
                break;
            }
        }
    }
    *outCount = count;
}
