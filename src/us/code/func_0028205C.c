#include "common.h"
#include "structs/camera.h"

s32 CCache__Deallocate(s32, s32);                /* extern */
s32 CCache__Flush(s32);                     /* extern */
s32 Sound__SetListener(sptr);                         /* extern */

/* Frees the pool's cameras, the main one first. */
void CCameraPool__FreeCameras(CCameraPool *pool) {
    pool->m_bOnlyUseMainCameraForSound = 1;
    Sound__SetListener(&pool->m_MainCamera);
    CCache__Flush(0);
    if (pool->m_pmeCameras != NULL) {
        CCache__Deallocate(0, pool->m_pmeCameras);
        pool->m_pmeCameras = NULL;
        pool->m_Cameras = NULL;
        pool->m_nCameras = 0;
    }
}
