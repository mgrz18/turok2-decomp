#include "common.h"
#include "structs/region.h"
#include "globals/scene.h"
#include "protos/scene.h"

extern u8 D_800C894C; /* bottom of the stack */
extern CVector3 D_8011AAB4;

#define NEAR(p) (((p)->x - D_8011AAB4.x) * ((p)->x - D_8011AAB4.x) + ((p)->z - D_8011AAB4.z) * ((p)->z - D_8011AAB4.z) < 167772.14f)

/* Marks the regions around the camera, recursing into neighbours that have
 * a corner within ~410 units of it. */
void Automap_RevealRegion(CGameRegion *region) {
    if (region == NULL || (u32) &region < (u32) &D_800C894C) {
        return;
    }
    if (!(region->m_wFlags & 0x400)) {
        CScene__SetLevelBit(&gScene, CScene__GetRegionIndex(&gScene, region), 1);
    }
    region->m_wFlags |= 0x8400;
    if (!(NEAR(region->m_pVertices[0]) || NEAR(region->m_pVertices[1]) || NEAR(region->m_pVertices[2]))) {
        return;
    }
    if (region->m_pNeighbours[0] != NULL && !(region->m_pNeighbours[0]->m_wFlags & 0x8000) && (REGION_WORD(region->m_pNeighbours[0]) & 6) != 2) {
        Automap_RevealRegion(region->m_pNeighbours[0]);
    }
    if (region->m_pNeighbours[1] != NULL && !(region->m_pNeighbours[1]->m_wFlags & 0x8000) && (REGION_WORD(region->m_pNeighbours[1]) & 6) != 2) {
        Automap_RevealRegion(region->m_pNeighbours[1]);
    }
    if (region->m_pNeighbours[2] != NULL && !(region->m_pNeighbours[2]->m_wFlags & 0x8000) && (REGION_WORD(region->m_pNeighbours[2]) & 6) != 2) {
        Automap_RevealRegion(region->m_pNeighbours[2]);
    }
}
