#include "common.h"
#include "structs/region.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "globals/game.h"

extern u8 D_800C894C; /* bottom of the stack */
#ifdef TARGET_PORT
/* the N64 compares the stack pointer with its main stack's bottom; the host asks its own thread's
 * stack (t2os_stack_low, port/runtime/t2os.c: the address comparison depended on the layout) */
int t2os_stack_low(const void *local);
#define T2_STACK_LOW(local, bottom) t2os_stack_low(local)
#else
#define T2_STACK_LOW(local, bottom) ((uptr) (local) < (uptr) (bottom))
#endif

#define PLAYER_POS gEngineApp_m_Game.m_Automap.m_vPlayerPos
#define NEAR(p) (((p)->x - PLAYER_POS.x) * ((p)->x - PLAYER_POS.x) + ((p)->z - PLAYER_POS.z) * ((p)->z - PLAYER_POS.z) < 167772.14f)

/* Marks the regions around the camera, recursing into neighbours that have
 * a corner within ~410 units of it. */
void Automap_RevealRegion(CGameRegion *region) {
    if (region == NULL || T2_STACK_LOW(&region, &D_800C894C)) {
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
