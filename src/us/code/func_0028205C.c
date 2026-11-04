#include "common.h"
#include "structs/camera.h"

s32 func_002052D8(s32, s32);                /* extern */
s32 func_002053A8(s32);                     /* extern */
s32 func_00275AE0(s32);                         /* extern */

/* Frees the pool's cameras, the main one first. */
void func_0028205C(CCameraPool *pool) {
    pool->m_bOnlyUseMainCameraForSound = 1;
    func_00275AE0(&pool->m_MainCamera);
    func_002053A8(0);
    if (pool->m_pmeCameras != NULL) {
        func_002052D8(0, pool->m_pmeCameras);
        pool->m_pmeCameras = NULL;
        pool->m_Cameras = NULL;
        pool->m_nCameras = 0;
    }
}
