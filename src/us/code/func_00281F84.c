#include "common.h"
#include "structs/camera.h"

s32 *func_00204EDC(s32, s32, s32, void *);          /* extern */
void func_00266C5C(s32, s32, s32);                  /* extern */
extern u8 D_800A9A00[];

/* Allocates the pool's arg1 cameras and clears them. */
void func_00281F84(CCameraPool *arg0, s32 arg1) {
    s32 *temp_v0;
    s32 temp_a0;
    s32 temp_s0;

    temp_s0 = arg1 * sizeof(CCamera);
    arg0->m_nCameras = arg1;
    temp_v0 = func_00204EDC(0, temp_s0, 0x23, D_800A9A00);
    arg0->m_pmeCameras = (struct CMemEntry *) temp_v0;
    temp_a0 = *temp_v0;
    arg0->m_Cameras = (struct CCamera *) temp_a0;
    func_00266C5C(temp_a0, 0, temp_s0);
}
