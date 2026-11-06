#include "common.h"
#include "structs/camera.h"

/* What this reads of a region: its flags. 0x10000 makes the ground tremble. */
typedef struct {
    /* 0x00 */ u8 _pad[0x44];
    /* 0x44 */ s32 m_Flags;
} RegionTremor;

extern u8 gScene[];
RegionTremor *CScene__GetRegionInfo(void *regions, struct CGameRegion *region);
f32 Random_Blend(f32 min, f32 max);          /* a random float between the two */

/* The tremor of the ground in a region that shakes: eases the camera's
 * tremor toward 0.5 degrees and 0.25 high there (toward nothing elsewhere)
 * and gives this frame's random offset of the view. */
void CCamera__UpdateTremor(CCamera *cam, CVector3 *offset) {
    f32 height = 0.0f;
    f32 angle = height;
    RegionTremor *region = CScene__GetRegionInfo(gScene, cam->m_ViewParams.m_pCurrentRegion);

    if (region == NULL) {
        offset->x = height;
        offset->y = height;
        offset->z = height;
        return;
    }
    if (region->m_Flags & 0x10000) {
        angle = 0.5f;
        height = 0.25f;
    }
    cam->m_TremorAngle += (angle - cam->m_TremorAngle) * 0.2f;
    cam->m_TremorHeight += (height - cam->m_TremorHeight) * 0.1f;
    offset->x = Random_Blend(-cam->m_TremorAngle * 0.017453294f, cam->m_TremorAngle * 0.017453294f);
    offset->y = Random_Blend(0.0f, cam->m_TremorHeight * 10.24f);
    offset->z = Random_Blend(-cam->m_TremorAngle * 0.017453294f, cam->m_TremorAngle * 0.017453294f);
}
