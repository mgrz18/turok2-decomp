#include "common.h"
#include "m2c_macros.h"
#include "structs/camera.h"

M2C_UNK func_002052D8(M2C_UNK, s32);                /* extern */
M2C_UNK func_002053A8(M2C_UNK);                     /* extern */
M2C_UNK func_00275AE0(s32);                         /* extern */

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
