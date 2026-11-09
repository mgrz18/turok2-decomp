#include "common.h"
#include "structs/camera.h"
#include "protos/cache.h"

void memset(s32, s32, s32);                  /* extern */
extern u8 D_800A9A00[];

/* Allocates the pool's arg1 cameras and clears them. */
void CCameraPool__AllocCameras(CCameraPool *arg0, s32 arg1) {
    s32 *temp_v0;
    s32 temp_a0;
    s32 temp_s0;

    temp_s0 = arg1 * sizeof(CCamera);
    arg0->m_nCameras = arg1;
    temp_v0 = CCache__Allocate(0, temp_s0, 0x23, D_800A9A00);
    arg0->m_pmeCameras = (struct CMemEntry *) temp_v0;
    temp_a0 = *temp_v0;
    arg0->m_Cameras = (struct CCamera *) temp_a0;
    memset(temp_a0, 0, temp_s0);
}
