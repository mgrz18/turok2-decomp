#include "common.h"
#include "structs/camera.h"

void Sound__SetListener(void *);
void CCache__Flush(s32);
void CCache__Deallocate(sptr, void *);

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
